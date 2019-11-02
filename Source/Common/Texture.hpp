
#pragma once

#include <string>


namespace FEngine
{
    /*
     * IMPORTANT: The textures MUST be a power of 2, specially on OpenGL ES2. 
     *              Otherwise they won't display properly on the screen. Even
     *              if one of the many textures is mal formed, it will screw
     *              up all others.
     * */

    class Texture
    {
    public:
        Texture();
        ~Texture();
        
        /**
         * Utility functions made static for global access.
         */
        //static bool LoadPngImage(const char * fileName, int &outWidth, int &outHeight, bool &outHasAlpha, unsigned char **outData);
        //static unsigned int LoadFromPixels32(unsigned int texWidth, unsigned int texHeight, bool hasAlpha, unsigned int * pixels );
        
        /**
         * @param	name		Unique string identifier used to get the texture from texture manager. eg: "Level1Atlas_1024" etc etc
         * @param	fileName	Name of the image file located on the disk.
         */
        bool        LoadFromFile        (std::string fileName);
        bool        LoadFromStream      (std::istream & pngDataStream);
        
        /**
         * Sets already loaded Gl texture (such as a font texture generated else where).
         * @param	textureID   GL id of an already loaded texture
         * @param	width       Width of the texture
         * @param	height      Height of the texture
         * @param	hasAlpla    Whether the texture has transparency in it. Default value is false.
         */
        
                
        void        SetName             (std::string name);
        std::string GetName             ();
        
        
        int         GetWidth            ();
        int         GetHeight           ();
        
    private:
        
        std::string	_name;
        
        int         _width;
        int         _height;
        int         _depth;
        bool        _hasAlpha;
    };
        
};


