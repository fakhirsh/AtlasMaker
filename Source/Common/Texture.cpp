
#include "Texture.hpp"
#include <png.h>
#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

namespace FEngine
{

    Texture::Texture ()
    {
        // -1 indicates invalid width/height
        _width      =   -1;
        _height     =   -1;
        _depth      =   -1;
        _hasAlpha   =   false;
    }


    Texture::~Texture ()
    {

    }

    int Texture::GetWidth ()
    {
        return _width;
    }

    int Texture::GetHeight ()
    {
        return _height;
    }

    void Texture::SetName (std::string name)
    {
        _name = name;
    }

    std::string Texture::GetName ()
    {
        return _name;
    }


    bool Texture::LoadFromFile (std::string fileName)
    {

        std::string pngStreamString;
        std::ifstream sourceFile( fileName.c_str() );

        //Source file loaded
        if( !sourceFile )
        {
            // App::Get()->GetLogger()->Print("Error: File not found: " + fileName, "Texture::LoadFromFile");
            std::cout << "Error: File not found: " + fileName << std::endl;
            return false;
        }

        //Get PNG file contents
        pngStreamString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );

        // Convert it into a data stream
        std::istringstream pngDataStream( pngStreamString );

        bool success = LoadFromStream(pngDataStream);
        if (!success)
        {
            return false;
        }

        return true;

    }



    /**
     * Please note that this function DOES NOT loads the all at once. This function
     * is called multiple times, in bursts, by the libpng library. So some kind of
     * stream handling functions are very helpful to advance the input stream read head.
     * @param   png_ptr     Pointer to png_struct initialized earlier
     * @param   destination Destination memory where the raw png data is to be copied
     * @param   bytesToRead Number of bytes that will be read in current burst/call.
     */
    void readFileCallback ( png_structp png_ptr, png_bytep destination, png_size_t bytesToRead )
    {
        png_voidp io_ptr = png_get_io_ptr( png_ptr );

        if( io_ptr == 0 )
        {
            return;
        }

        png_voidp a = png_get_io_ptr(png_ptr);
        //Cast the pointer to std::istream* and read 'bytesToRead' bytes into 'destination'
        ((std::istream*)a)->read((char*)destination, bytesToRead);
    }

    /**
     * Parses PNG data from stream and extracts raw bitmap pixels for further usage.
     * @param   pngDataStream   Input stream containing PNG data read earlier maybe from a file.
     * @param   bitmapData      Output bitmap data which could be forwarded to OpenGL for texture generation.
     */
    bool Texture::LoadFromStream (std::istream & pngDataStream)
    {
        const int PNG_SIG_BYTES = 8;
        char pngSignature[PNG_SIG_BYTES];
        pngDataStream.read(pngSignature, PNG_SIG_BYTES * sizeof(char));

        if(!png_check_sig( (png_bytep)pngSignature, PNG_SIG_BYTES) )
        {
            return false;
        }

        /**
         * Create and initialize the png_struct
         * with the desired error handler
         * functions.  If you want to use the
         * default stderr and longjump method,
         * you can supply NULL for the last
         * three parameters.  We also supply the
         * the compiler header file version, so
         * that we know if the application
         * was compiled with a compatible version
         * of the library.  REQUIRED
         */
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr == NULL)
        {
            return false;
        }

        /**
         * Allocate/initialize the memory
         * for image information.  REQUIRED. */
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL)
        {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return false;
        }

        /**
         * Set error handling if you are
         * using the setjmp/longjmp method
         * (this is the normal method of
         * doing things with libpng).
         * REQUIRED unless you  set up
         * your own error handlers in
         * the png_create_read_struct()
         * earlier.
         */
        if (setjmp(png_jmpbuf(png_ptr))) {
            /* Free all of the memory associated
             * with the png_ptr and info_ptr */
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return false;
        }

        /**
         * Set custom input stream READER / handler
         */
        png_set_read_fn( png_ptr, (void*)&pngDataStream, readFileCallback );

        /* If we have already
         * read some of the signature */
        png_set_sig_bytes( png_ptr, 8 );

        png_read_info( png_ptr, info_ptr);

        int color_type, interlace_type;

        png_get_IHDR( png_ptr, info_ptr, (png_uint_32*)&_width, (png_uint_32*)&_height, &_depth, &color_type, &interlace_type, NULL, NULL );

        switch(color_type)
        {
            case PNG_COLOR_TYPE_RGB:
                _hasAlpha = false;
                break;
            case PNG_COLOR_TYPE_RGBA:
                _hasAlpha = true;
                break;
            default:
                return false;
                break;
        }

        png_size_t cols = png_get_rowbytes(png_ptr, info_ptr);

        png_bytepp row_pp = new png_bytep[_height];
        char * bitmapData = new char[ cols * _height ];

        for( int i = 0; i < _height; ++i )
        {
            // note that png is ordered top to
            // bottom, but OpenGL expect it bottom to top
            // so the order or swapped
            row_pp[_height - i - 1] = (png_bytep)&((char *)bitmapData)[ i * cols ];
        }

        png_read_image( png_ptr, row_pp );
        png_read_end( png_ptr, info_ptr );

        png_destroy_read_struct( &png_ptr, &info_ptr, 0 );

        delete [] row_pp;

        //RendererPtr renderer = App::Get()->GetRenderer();
        //_textureID = renderer->LoadTextureFromPixels32(_width, _height, _hasAlpha, (unsigned int *)bitmapData);

        delete [] bitmapData;

        return true;
    }

/*
 *
 *    unsigned int Texture::LoadFromPixels32 (unsigned int texWidth, unsigned int texHeight, bool hasAlpha, unsigned int * pixels )
 *    {
 *        Log * log = App::Get()->GetLogger();
 *
 *        unsigned int _textureID;
 *
 *        glGenTextures(1, &_textureID);
 *        glBindTexture(GL_TEXTURE_2D, _textureID);
 *        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 *        glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, texWidth, texHeight, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels);
 *
 *        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 *        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 *        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
 *        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 *
 *        glBindTexture(GL_TEXTURE_2D, 0);
 *
 *        return _textureID;
 *    }
 *
 */

//-----------------------------------------------------------------------------------------

    /**
     * Sample function showing how to generate and load textures from memory
     */
/*
 *    bool LoadTexturesFromMem ()
 *    {
 *        GLuint _uiTexture;
 *
 *        glGenTextures(1, &_uiTexture);
 *
 *        // Binds this texture handle so we can load the data into it
 *        glBindTexture(GL_TEXTURE_2D, _uiTexture);
 *
 *        const int TEX_SIZE = 128;
 *        // Creates the data as a 32bits integer array (8bits per component)
 *        GLuint* pTexData = new GLuint[TEX_SIZE*TEX_SIZE];
 *        for (int i=0; i<TEX_SIZE; i++)
 *            for (int j=0; j<TEX_SIZE; j++)
 *            {
 *                // Fills the data with a fancy pattern
 *                GLuint col = (255<<24) + ((255-j*2)<<16) + ((255-i)<<8) + (255-i*2);
 *                if ( ((i*j)/8) % 2 ) col = (GLuint) (255<<24) + (255<<16) + (0<<8) + (255);
 *                pTexData[j*TEX_SIZE+i] = col;
 *            }
 *
 *        //[>
 *         //glTexImage2D loads the texture data into the texture object.
 *         //void glTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
 *         //GLint border, GLenum format, GLenum type, const GLvoid *pixels );
 *         //target must be GL_TEXTURE_2D.
 *         //level specify the mipmap level we want to upload.
 *         //internalformat and format must be the same. Here we use GL_RGBA for 4 component colors (r,g,b,a).
 *         //We could use GL_RGB, GL_ALPHA, GL_LUMINANCE, GL_LUMINANCE_ALPHA to use different color component combinations.
 *         //width, height specify the size of the texture. Both of the dimensions must be power of 2.
 *         //border must be 0.
 *         //type specify the format of the data. We use GL_UNSIGNED_BYTE to describe a color component as an unsigned byte.
 *         //So a pixel is described by a 32bits integer.
 *         //We could also use GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, and GL_UNSIGNED_SHORT_5_5_5_1
 *         //to specify the size of all 3 (or 4) color components. If we used any of these 3 constants,
 *         //a pixel would then be described by a 16bits integer.
 *         
 *        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexData);
 *
 *        [>
 *         //glTexParameterf is used to set the texture parameters
 *         //void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
 *         //target must be GL_TEXTURE_2D.
 *         //pname is the parameter name we want to modify.
 *         //If pname is GL_TEXTURE_MIN_FILTER, param is used to set the way the texture is rendered when made smaller.
 *         //We can tell OpenGL to interpolate between the pixels in a mipmap level but also between different mipmap levels.
 *         //We are not using mipmap interpolation here because we didn't defined the mipmap levels of our texture.
 *
 *         //If pname is GL_TEXTURE_MAG_FILTER, param is used to set the way the texture is rendered when made bigger.
 *         //Here we can only tell OpenGL to interpolate between the pixels of the first mipmap level.
 *
 *         //if pname is GL_TEXTURE_WRAP_S or GL_TEXTURE_WRAP_T, then param sets the way a texture tiles in both directions.
 *         //The default if GL_REPEAT to wrap the texture (repeat it). We could also set it to GL_CLAMP or GL_CLAMP_TO_EDGE
 *         //to clamp the texture.
 *
 *         //On OpenGL ES 1.1 and 2.0, if pname is GL_GENERATE_MIPMAP, param tells OpenGL to create mipmap levels automatically.
 *         
 *        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
 *        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 *
 *        // Deletes the texture data, it's now in OpenGL memory
 *        delete [] pTexData;
 *
 *        glBindTexture(GL_TEXTURE_2D, 0);
 *
 *        return true;
 *
 *    }
 */

};
