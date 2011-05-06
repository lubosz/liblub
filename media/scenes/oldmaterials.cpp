class ShadowMapSimple : public Material {
 public:
	ShadowMapSimple(unsigned width, unsigned height) {
		init();
		addTexture(TextureFactory::Instance(). depthTexture(width, height,
                "shadowMap"));
		shaderProgram->attachVertFrag("Shadow/Simple");
		done();
  }
	void uniforms() {}
};
class ShadowMapPCF : public Material {
 public:
	ShadowMapPCF(unsigned width, unsigned height) {
		init();
		Texture * shadowMap =
		        TextureFactory::Instance().depthTexture(width, height, "shadowMap");
		addTexture(shadowMap);
		shaderProgram->attachVertFrag("Shadow/PCF");
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(1.0/1200, "yPixelOffset");
		shaderProgram->setUniform(1.0/1920, "xPixelOffset");
	}
};
class ConvolutionMaterial : public Material {
 public:
	ConvolutionMaterial() {
		init();
		addTexture("bunny.png", "BaseImage");
		shaderProgram->attachShader("Texture/texture.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("Post/old/smoothing.frag", GL_FRAGMENT_SHADER);
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getHandle();
		// array of offsets for accessing the base image
		glUniform2f(glGetUniformLocation(program, "Offset"), 0.0, 0.0);
		// size of kernel (width * height) for this execution
		glUniform1i(glGetUniformLocation(program, "KernelSize"), 9);
		// value for each location in the convolution kernel
		glUniform4f(glGetUniformLocation(program, "ScaleFactor"), 1.0, 1.0, 1.0, 1.0);
	}
};
class SSS : public Material {
 public:
	SSS() {
		init();
		shaderProgram->attachVertFrag("Stuff/sss");
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(1.0, "SpecPower");
		shaderProgram->setUniform(1.0, "RimScalar");
		shaderProgram->setUniform(1.0, "MaterialThickness");
		shaderProgram->setUniform(
		        QVector3D(.1, .2, .3), "ExtinctionCoefficient");
		shaderProgram->setUniform(
		        QVector4D(.1, .2, .3, 1), "BaseColor");
		shaderProgram->setUniform(
		        QVector4D(.1, .2, .3, 1), "SpecColor");
	}
};
class HatchingMat : public Material {
 public:
	HatchingMat() {
		init();
		shaderProgram->attachVertFrag("Procedural/hatching");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getHandle();
		glUniform1f(glGetUniformLocation(program, "Time"), 1.0);
	}
};
class GeomTest : public Material {
 public:
	GeomTest() {
		init();
		shaderProgram->attachVertFragGeom("Geometry/test");
		done();
  }
	void uniforms() {}
};
class FogMaterial : public Material {
 public:
	FogMaterial() {
		init();
		shaderProgram->attachVertFrag("Stuff/Fog");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getHandle();

		glUniform1f(glGetUniformLocation(program, "uFogPos"), 0.8);
		glUniform3f(glGetUniformLocation(program, "uEyePos"), 0.0, 0.0, 1.0);
	}
};
