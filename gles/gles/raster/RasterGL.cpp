/* Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 * Copyright (c) 2012 cocos2d-x.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "RasterGL.h"
#include "CCPointExtension.h"
#include "CCShaderCache.h"
#include "CCDrawingPrimitives.h"
//#include "CCGL.h"
#include "gl/glew.h"


// ccVertex2F == CGPoint in 32-bits, but not in 64-bits (OS X)
// that's why the "v2f" functions are needed
static ccVertex2F v2fzero = {0.0f,0.0f};

static inline ccVertex2F v2f(float x, float y)
{
    ccVertex2F ret = {x, y};
	return ret;
}

static inline ccVertex2F v2fadd(const ccVertex2F &v0, const ccVertex2F &v1)
{
	return v2f(v0.x+v1.x, v0.y+v1.y);
}

static inline ccVertex2F v2fsub(const ccVertex2F &v0, const ccVertex2F &v1)
{
	return v2f(v0.x-v1.x, v0.y-v1.y);
}

static inline ccVertex2F v2fmult(const ccVertex2F &v, float s)
{
	return v2f(v.x * s, v.y * s);
}

static inline ccVertex2F v2fperp(const ccVertex2F &p0)
{
	return v2f(-p0.y, p0.x);
}

static inline ccVertex2F v2fneg(const ccVertex2F &p0)
{
	return v2f(-p0.x, - p0.y);
}

static inline float v2fdot(const ccVertex2F &p0, const ccVertex2F &p1)
{
	return  p0.x * p1.x + p0.y * p1.y;
}

static inline ccVertex2F v2fforangle(float _a_)
{
	return v2f(cosf(_a_), sinf(_a_));
}

static inline ccVertex2F v2fnormalize(const ccVertex2F &p)
{
	CCPoint r = ccpNormalize(ccp(p.x, p.y));
	return v2f(r.x, r.y);
}

static inline ccVertex2F __v2f(const CCPoint &v)
{
//#ifdef __LP64__
	return v2f(v.x, v.y);
// #else
// 	return * ((ccVertex2F*) &v);
// #endif
}

static inline ccTex2F __t(const ccVertex2F &v)
{
	return *(ccTex2F*)&v;
}

// implementation of CCDrawNode

RasterGL::RasterGL()
: m_uVao(0)
, m_uVbo(0)
, m_uBufferCapacity(0)
, m_nBufferCount(0)
, m_pBuffer(NULL)
, m_bDirty(false)
{
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
}

RasterGL::~RasterGL()
{
    free(m_pBuffer);
    m_pBuffer = NULL;
    
    glDeleteBuffers(1, &m_uVbo);
    m_uVbo = 0;
    
#if CC_TEXTURE_ATLAS_USE_VAO      
    glDeleteVertexArrays(1, &m_uVao);
    m_uVao = 0;
#endif
}

RasterGL* RasterGL::create()
{
    RasterGL* pRet = new RasterGL();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

void RasterGL::ensureCapacity(unsigned int count)
{
    if(m_nBufferCount + count > m_uBufferCapacity)
    {
		m_uBufferCapacity += MAX(m_uBufferCapacity, count);
		m_pBuffer = (ccV2F_C4B_T2F*)realloc(m_pBuffer, m_uBufferCapacity*sizeof(ccV2F_C4B_T2F));
	}
}

bool RasterGL::init()
{
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;

    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionLengthTexureColor));
    
    ensureCapacity(512);
    
#if CC_TEXTURE_ATLAS_USE_VAO    
    glGenVertexArrays(1, &m_uVao);
    ccGLBindVAO(m_uVao);
#endif
    
    glGenBuffers(1, &m_uVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)* m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
    
    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
    
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
#if CC_TEXTURE_ATLAS_USE_VAO 
    ccGLBindVAO(0);
#endif
    
    m_bDirty = true;
    
    return true;
}

void RasterGL::render()
{
    if (m_bDirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
        m_bDirty = false;
    }
#if CC_TEXTURE_ATLAS_USE_VAO     
    ccGLBindVAO(m_uVao);
#else
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
    // vertex
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
    
    // color
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
    
    // texcood
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
#endif

    glDrawArrays(GL_TRIANGLES, 0, m_nBufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //CC_INCREMENT_GL_DRAWS(1);
}

void RasterGL::draw()
{
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    
    getShaderProgram()->use();
    getShaderProgram()->setUniformsForBuiltins();
    
    render();
}

void RasterGL::drawDot(const CCPoint &pos, float radius, const ccColor4F &color)
{
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
	
	ccV2F_C4B_T2F a = {{pos.x - radius, pos.y - radius}, ccc4BFromccc4F(color), {-1.0, -1.0} };
	ccV2F_C4B_T2F b = {{pos.x - radius, pos.y + radius}, ccc4BFromccc4F(color), {-1.0,  1.0} };
	ccV2F_C4B_T2F c = {{pos.x + radius, pos.y + radius}, ccc4BFromccc4F(color), { 1.0,  1.0} };
	ccV2F_C4B_T2F d = {{pos.x + radius, pos.y - radius}, ccc4BFromccc4F(color), { 1.0, -1.0} };
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
    ccV2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    ccV2F_C4B_T2F_Triangle triangle1 = {a, c, d};
	triangles[0] = triangle0;
	triangles[1] = triangle1;
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;
}

void RasterGL::drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color)
{
    unsigned int vertex_count = 6*3;
    ensureCapacity(vertex_count);
	
	ccVertex2F a = __v2f(from);
	ccVertex2F b = __v2f(to);
	
	
	ccVertex2F n = v2fnormalize(v2fperp(v2fsub(b, a)));
	ccVertex2F t = v2fperp(n);
	
	ccVertex2F nw = v2fmult(n, radius);
	ccVertex2F tw = v2fmult(t, radius);
	ccVertex2F v0 = v2fsub(b, v2fadd(nw, tw));
	ccVertex2F v1 = v2fadd(b, v2fsub(nw, tw));
	ccVertex2F v2 = v2fsub(b, nw);
	ccVertex2F v3 = v2fadd(b, nw);
	ccVertex2F v4 = v2fsub(a, nw);
	ccVertex2F v5 = v2fadd(a, nw);
	ccVertex2F v6 = v2fsub(a, v2fsub(nw, tw));
	ccVertex2F v7 = v2fadd(a, v2fadd(nw, tw));
	
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
	
    ccV2F_C4B_T2F_Triangle triangles0 = {
        {v0, ccc4BFromccc4F(color), __t(v2fneg(v2fadd(n, t)))},
        {v1, ccc4BFromccc4F(color), __t(v2fsub(n, t))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[0] = triangles0;
	
    ccV2F_C4B_T2F_Triangle triangles1 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v1, ccc4BFromccc4F(color), __t(v2fsub(n, t))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[1] = triangles1;
	
    ccV2F_C4B_T2F_Triangle triangles2 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[2] = triangles2;

    ccV2F_C4B_T2F_Triangle triangles3 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n))},
        {v5, ccc4BFromccc4F(color), __t(n) },
    };
    triangles[3] = triangles3;

    ccV2F_C4B_T2F_Triangle triangles4 = {
        {v6, ccc4BFromccc4F(color), __t(v2fsub(t, n))},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n)) },
        {v5, ccc4BFromccc4F(color), __t(n)},
    };
	triangles[4] = triangles4;

    ccV2F_C4B_T2F_Triangle triangles5 = {
        {v6, ccc4BFromccc4F(color), __t(v2fsub(t, n))},
        {v7, ccc4BFromccc4F(color), __t(v2fadd(n, t))},
        {v5, ccc4BFromccc4F(color), __t(n)},
    };
	triangles[5] = triangles5;
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;
}

void RasterGL::drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor)
{
    struct ExtrudeVerts {ccVertex2F offset, n;};
	struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts)*count);
	memset(extrude, 0, sizeof(struct ExtrudeVerts)*count);
	
	for(unsigned int i = 0; i < count; i++)
    {  
		ccVertex2F v0 = __v2f(verts[(i-1+count)%count]);
		ccVertex2F v1 = __v2f(verts[i]);
		ccVertex2F v2 = __v2f(verts[(i+1)%count]);
        
		ccVertex2F n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
		ccVertex2F n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));
		
		ccVertex2F offset = v2fmult(v2fadd(n1, n2), 1.0/(v2fdot(n1, n2) + 1.0));
        struct ExtrudeVerts tmp = {offset, n2};
		extrude[i] = tmp;
	}
	
	bool outline = (fillColor.a > 0.0 && borderWidth > 0.0);
	
	unsigned int triangle_count = 3*count - 2;
	unsigned int vertex_count = 3*triangle_count;
    ensureCapacity(vertex_count);
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
	ccV2F_C4B_T2F_Triangle *cursor = triangles;
	
	float inset = (outline == 0.0 ? 0.5 : 0.0);
	for(unsigned int i = 0; i < count-2; i++)
    {
		ccVertex2F v0 = v2fsub(__v2f(verts[0  ]), v2fmult(extrude[0  ].offset, inset));
		ccVertex2F v1 = v2fsub(__v2f(verts[i+1]), v2fmult(extrude[i+1].offset, inset));
		ccVertex2F v2 = v2fsub(__v2f(verts[i+2]), v2fmult(extrude[i+2].offset, inset));
		
        ccV2F_C4B_T2F_Triangle tmp = {
            {v0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
            {v1, ccc4BFromccc4F(fillColor), __t(v2fzero)},
            {v2, ccc4BFromccc4F(fillColor), __t(v2fzero)},
        };

		*cursor++ = tmp;
	}
	
	for(unsigned int i = 0; i < count; i++)
    {
		int j = (i+1)%count;
		ccVertex2F v0 = __v2f(verts[i]);
		ccVertex2F v1 = __v2f(verts[j]);
		
		ccVertex2F n0 = extrude[i].n;
		
		ccVertex2F offset0 = extrude[i].offset;
		ccVertex2F offset1 = extrude[j].offset;
		
		if(outline)
        {
			ccVertex2F inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
			ccVertex2F inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
			ccVertex2F outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
			ccVertex2F outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));
			
            ccV2F_C4B_T2F_Triangle tmp1 = {
                {inner0, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {inner1, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {outer1, ccc4BFromccc4F(borderColor), __t(n0)}
            };
			*cursor++ = tmp1;

            ccV2F_C4B_T2F_Triangle tmp2 = {
                {inner0, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {outer0, ccc4BFromccc4F(borderColor), __t(n0)},
                {outer1, ccc4BFromccc4F(borderColor), __t(n0)}
            };
			*cursor++ = tmp2;
		}
        else {
			ccVertex2F inner0 = v2fsub(v0, v2fmult(offset0, 0.5));
			ccVertex2F inner1 = v2fsub(v1, v2fmult(offset1, 0.5));
			ccVertex2F outer0 = v2fadd(v0, v2fmult(offset0, 0.5));
			ccVertex2F outer1 = v2fadd(v1, v2fmult(offset1, 0.5));
			
            ccV2F_C4B_T2F_Triangle tmp1 = {
                {inner0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {inner1, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {outer1, ccc4BFromccc4F(fillColor), __t(n0)}
            };
			*cursor++ = tmp1;

            ccV2F_C4B_T2F_Triangle tmp2 = {
                {inner0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {outer0, ccc4BFromccc4F(fillColor), __t(n0)},
                {outer1, ccc4BFromccc4F(fillColor), __t(n0)}
            };
			*cursor++ = tmp2;
		}
	}
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;

    free(extrude);
}

void RasterGL::drawTriangle( const CCPoint &p1, const CCPoint &p2, const CCPoint &p3, const ccColor4F &color )
{
	unsigned int vertex_count = 2 * 3;
	ensureCapacity( vertex_count );
	ccColor4B col = ccc4BFromccc4F( color );
	ccV2F_C4B_T2F_Triangle triangle =
	{
		{ vertex2( p1.x, p1.y), col, __t( v2fzero) },
		{ vertex2( p2.x, p2.y), col, __t( v2fzero ) },
		{ vertex2( p3.x, p3.y), col, __t( v2fzero ) }
	};
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
	//ccV2F_C4B_T2F_Triangle triangle = { a, b, c };
	triangles[0] = triangle;
	m_nBufferCount += vertex_count;
	m_bDirty = true;
	return;
}

void RasterGL::clear()
{
    m_nBufferCount = 0;
    m_bDirty = true;
}

std::vector<CCPoint*> g_pintArray;
RasterGL * pthis = NULL;
void __stdcall vertexCallback(GLdouble *vertex)
{
	const GLdouble *pointer;
	CCPoint *p = new CCPoint();
	p->x = vertex[0];
	p->y = vertex[1];
	g_pintArray.push_back( p );

	//pointer = (GLdouble *) vertex;
	//glColor3dv(pointer+3);
	//glVertex3dv((GLdouble*)vertex);

	//g_pintArray
}

void __stdcall beginCallback( GLenum which )
{

}

void __stdcall endCallback( )
{
	static ccColor4F green ={0,1,0,1};
	int count = g_pintArray.size();
	//const int nCount=100;
	//static CCPoint circle[nCount];
	//circle[0].x = 50;
	//circle[0].y = 100;
	//circle[1].x = 0;
	//circle[1].y = 0;
	//circle[2].x = -50;
	//circle[2].y = 100;
	//circle[3].x = -100;
	//circle[3].y = 50;
	//circle[4].x = -150;
	//circle[4].y = 100;
	//drawPolygon( (CCPoint*)(g_pintArray[0]), count,green, 1, green );

	CCPoint * pdata = new CCPoint[count];
	for ( int i = 0; i < count; i++ )
	{
		pdata[i].x = g_pintArray[i]->x;
		pdata[i].y = g_pintArray[i]->y;
	}

	ccDrawSolidPoly( pdata, count, green );
	delete []pdata;
	std::vector<CCPoint*>::iterator iter = g_pintArray.begin();
	std::vector<CCPoint*>::iterator end = g_pintArray.end();
	for ( ; iter != end; iter++ )
	{
		delete *iter;
	}
	g_pintArray.clear();
	
	
}

void __stdcall errorCallback( GLenum errorCode )
{
	const GLubyte *estring;
	//estring = gluErrorString( errorCode );
	//fprintf( stderr, "Tessellation Error: %s\n", estring );
	exit( 0 );
}

void __stdcall combineCallback(GLdouble coords[3], 
		GLdouble *vertex_data[4],
		GLfloat weight[4], GLdouble **dataOut )
{
	GLdouble *vertex;
	int i;

	vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));

	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	for (i = 3; i < 6; i++)
		vertex[i] = weight[0] * vertex_data[0][i] 
					+ weight[1] * vertex_data[1][i]
					+ weight[2] * vertex_data[2][i] 
					+ weight[3] * vertex_data[3][i];
	*dataOut = vertex;

	CCPoint *p = new CCPoint();
	p->x = vertex[0];
	p->y = vertex[1];
	g_pintArray.push_back( p );
	
	
}

//GLdouble star[5][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	125.0, 200.0, 0.0, 1.0, 1.0, 0.0,
//	200.0, 50.0, 0.0, 0.0, 1.0, 1.0,
//	50.0, 150.0, 0.0, 1.0, 0.0, 0.0,
//	200.0, 150.0, 0.0, 0.0, 1.0, 0.0
//};

//GLdouble star[5][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	125.0, 200.0, 0.0, 1.0, 1.0, 0.0,
//	200.0, 50.0, 0.0, 0.0, 1.0, 1.0,
//	50.0, 150.0, 0.0, 1.0, 0.0, 0.0,
//	200.0, 150.0, 0.0, 0.0, 1.0, 0.0
//};

//GLdouble star[8][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	150.0, 50.0, 0.0, 1.0, 0.0, 0.0,
//	150.0, 150.0, 0.0, 0.0, 1.0, 1.0,
//	50.0, 150.0, 0.0, 1.0, 1.0, 0.0,
//	70.0, 70.0, 0.0, 1.0, 0.0, 0.0,
//	70.0, 120.0, 0.0, 0.0, 1.0, 1.0,
//	120.0, 120.0, 0.0, 1.0, 1.0, 0.0,
//	120.0, 70.0, 0.0, 1.0, 0.0, 1.0
//};

GLdouble star[10][6] =
{
	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
	150.0, 50.0, 0.0, 1.0, 0.0, 0.0,
	//150.0, 70.0, 0.0, 0.0, 1.0, 1.0,
	//120.0, 70.0, 0.0, 1.0, 0.0, 1.0,
	70.0, 70.0, 0.0, 1.0, 0.0, 0.0,
	70.0, 120.0, 0.0, 0.0, 1.0, 1.0,
	120.0, 120.0, 0.0, 1.0, 1.0, 0.0,
	120.0, 70.0, 0.0, 1.0, 0.0, 1.0,
	150.0, 70.0, 0.0, 0.0, 1.0, 1.0,
	150.0, 150.0, 0.0, 0.0, 1.0, 1.0,
	50.0, 150.0, 0.0, 1.0, 1.0, 0.0
};

//GLdouble star[8][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	50.0, 200.0, 0.0, 1.0, 0.0, 0.0,
//	100.0, 200.0, 0.0, 0.0, 1.0, 1.0,
//	100.0, 100.0, 0.0, 1.0, 1.0, 0.0,
//	150.0, 100.0, 0.0, 1.0, 0.0, 0.0,
//	150.0, 200.0, 0.0, 0.0, 1.0, 1.0,
//	200.0, 200.0, 0.0, 1.0, 1.0, 0.0,
//	200.0, 50.0, 0.0, 1.0, 0.0, 1.0
//};



void RasterGL::beginPolygon()
{
	tobj = gluNewTess();
	pthis = this;
	gluTessCallback(tobj, GLU_TESS_VERTEX, (void (__stdcall *)())vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN,  (void (__stdcall *)())beginCallback);
	gluTessCallback(tobj, GLU_TESS_END, (void (__stdcall *)())endCallback);
	gluTessCallback(tobj, GLU_TESS_ERROR, (void (__stdcall *)())errorCallback);
	gluTessCallback(tobj, GLU_TESS_COMBINE, (void (__stdcall *)())combineCallback);

	gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);
	gluTessBeginPolygon(tobj, NULL);
	gluTessBeginContour(tobj);
	gluTessVertex(tobj, star[0], star[0]);
	gluTessVertex(tobj, star[1], star[1]);
	gluTessVertex(tobj, star[2], star[2]);
	gluTessVertex(tobj, star[3], star[3]);
	gluTessVertex(tobj, star[4], star[4]);
	gluTessVertex(tobj, star[5], star[5]);
	gluTessVertex(tobj, star[6], star[6]);
	gluTessVertex(tobj, star[7], star[7]);
	gluTessVertex(tobj, star[8], star[8]);
	gluTessVertex(tobj, star[9], star[9]);
	//gluTessVertex(tobj, star[10], star[10]);
	//gluTessVertex(tobj, star[11], star[11]);
	gluTessEndContour(tobj);
	gluTessEndPolygon(tobj);

	return;
}

void RasterGL::endPolygon()
{
	 gluDeleteTess(tobj);

	 //static ccColor4F green ={0,1,0,1};
	 //int count = g_pintArray.size();
	 ////const int nCount=100;
	 ////static CCPoint circle[nCount];
	 ////circle[0].x = 50;
	 ////circle[0].y = 100;
	 ////circle[1].x = 0;
	 ////circle[1].y = 0;
	 ////circle[2].x = -50;
	 ////circle[2].y = 100;
	 ////circle[3].x = -100;
	 ////circle[3].y = 50;
	 ////circle[4].x = -150;
	 ////circle[4].y = 100;
	 ////drawPolygon( (CCPoint*)(g_pintArray[0]), count,green, 1, green );

	 //CCPoint * pdata = new CCPoint[count];
	 //for ( int i = 0; i < count; i++ )
	 //{
		// pdata[i].x = g_pintArray[i]->x;
		// pdata[i].y = g_pintArray[i]->y;
	 //}

	 //ccDrawSolidPoly( pdata, count, green );
	 //delete []pdata;

}

void RasterGL::drawAllPolygon()
{

}


ccBlendFunc RasterGL::getBlendFunc() const
{
    return m_sBlendFunc;
}

void RasterGL::setBlendFunc(const ccBlendFunc &blendFunc)
{
    m_sBlendFunc = blendFunc;
}
