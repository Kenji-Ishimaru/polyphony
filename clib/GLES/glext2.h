#ifndef __glext2_h_
#define __glext2_h_

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*
 * OES extension tokens
 *------------------------------------------------------------------------*/
// Shading model extension
#define GL_COOK_TORRANCE_GOLD              0x1D02
#define GL_COOK_TORRANCE_SILVER            0x1D03

/*------------------------------------------------------------------------*
 * OES extension functions
 *------------------------------------------------------------------------*/
void glGenCTFresnelTableOES (GLenum format, GLfloat f, GLsizei size);  // r,g,b,table size color
void glGenCTRoughnessTableOES (GLfloat r);  //  roughness, table size


#ifdef __cplusplus
}
#endif

#endif

