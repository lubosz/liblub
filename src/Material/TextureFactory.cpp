/*
 * Copyright © 2010 Lubosz Sarnecki
 * TextureFactory.cpp
 *
 *  Created on: Mar 25, 2010
 */

#include "Material/TextureFactory.h"
#include "Material/Textures.h"

TextureFactory::TextureFactory() {
	textureCount = 0;

	textureEnums = {
			GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4,
			GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8, GL_TEXTURE9,
			GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14,
			GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19,
			GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23, GL_TEXTURE24,
			GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29,
			GL_TEXTURE30, GL_TEXTURE31
	};
}

TextureFactory::~TextureFactory() {
	// TODO(bmonkey): Auto-generated destructor stub
}
Texture * TextureFactory::depthTexture(
        GLuint width, GLuint height, string name) {
	Texture * texture =
	        new DepthTexture(width, height, name, textureEnums[textureCount]);
	textureCount++;
	return texture;
}
Texture * TextureFactory::colorTexture(
        GLuint width, GLuint height, string name) {
	Texture * texture =
	        new ColorTexture(width, height, name, textureEnums[textureCount]);
	textureCount++;
	return texture;
}
Texture * TextureFactory::load(string filename, string name) {
	Texture * texture =
	        new TextureFile(filename, textureEnums[textureCount], name);
	textureCount++;
	return texture;
}
Texture * TextureFactory::loadCubeMap(string filename, string name) {
	Texture * texture =
	        new CubeTextureFile(filename, textureEnums[textureCount], name, true);
	textureCount++;
	return texture;
}

Texture * TextureFactory::splatTexture(string name, int resolution) {
	Texture * texture =
	        new SplatTexture(textureEnums[textureCount], name, resolution);
	textureCount++;
	return texture;
}
