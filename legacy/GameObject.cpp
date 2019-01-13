#include "GameObject.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"

void GameObject::Movement(glm::vec3 _Position)
{
	model = glm::translate(model, _Position);
}

void GameObject::Reset()
{
	model = glm::mat4(1.0);
}

GameObject::GameObject(std::string _Texture, std::string _Obj)
{
	Object = new VertexArray(_Obj);
	Tex = new Texture(_Texture);
	model = glm::mat4(1.0);
	
}
GameObject::GameObject(std::string _Texture, std::string _Obj, std::string _Normal)
{
	Object = new VertexArray(_Obj);
	Tex = new Texture(_Texture);
	Normal = new Texture(_Normal);
	model = glm::mat4(1.0);
}

GameObject::~GameObject()
{
}

int coplanar_tri_tri(double N[3], double V0[3], double V1[3], double V2[3],
	double U0[3], double U1[3], double U2[3]);

// some vector macros 
#define CROSS(dest,v1,v2)                       \
               dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
               dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
               dest[2]=v1[0]*v2[1]-v1[1]*v2[0];



#define   sVpsV_2( Vr, s1,  V1,s2, V2);\
	{\
  Vr[0] = s1*V1[0] + s2*V2[0];\
  Vr[1] = s1*V1[1] + s2*V2[1];\
}\

#define myVpV(g,v2,v1);\
{\
	g[0] = v2[0]+v1[0];\
	g[1] = v2[1]+v1[1];\
	g[2] = v2[2]+v1[2];\
	}\

#define myVmV(g,v2,v1);\
{\
	g[0] = v2[0]-v1[0];\
	g[1] = v2[1]-v1[1];\
	g[2] = v2[2]-v1[2];\
	}\

// 2D intersection of segment and triangle.
#define seg_collide3( q, r)\
{\
	p1[0]=SF*P1[0];\
	p1[1]=SF*P1[1];\
	p2[0]=SF*P2[0];\
	p2[1]=SF*P2[1];\
	det1 = p1[0]*q[1]-q[0]*p1[1];\
	gama1 = (p1[0]*r[1]-r[0]*p1[1])*det1;\
	alpha1 = (r[0]*q[1] - q[0]*r[1])*det1;\
	alpha1_legal = (alpha1>=0) && (alpha1<=(det1*det1)  && (det1!=0));\
	det2 = p2[0]*q[1] - q[0]*p2[1];\
	alpha2 = (r[0]*q[1] - q[0]*r[1]) *det2;\
	gama2 = (p2[0]*r[1] - r[0]*p2[1]) * det2;\
	alpha2_legal = (alpha2>=0) && (alpha2<=(det2*det2) && (det2 !=0));\
	det3=det2-det1;\
	gama3=((p2[0]-p1[0])*(r[1]-p1[1]) - (r[0]-p1[0])*(p2[1]-p1[1]))*det3;\
	if (alpha1_legal)\
	{\
		if (alpha2_legal)\
		{\
			if ( ((gama1<=0) && (gama1>=-(det1*det1))) || ((gama2<=0) && (gama2>=-(det2*det2))) || (gama1*gama2<0)) return 12;\
		}\
		else\
		{\
			if ( ((gama1<=0) && (gama1>=-(det1*det1))) || ((gama3<=0) && (gama3>=-(det3*det3))) || (gama1*gama3<0)) return 13;\
			}\
	}\
	else\
	if (alpha2_legal)\
	{\
		if ( ((gama2<=0) && (gama2>=-(det2*det2))) || ((gama3<=0) && (gama3>=-(det3*det3))) || (gama2*gama3<0)) return 23;\
		}\
	return 0;\
	}

int GameObject::tr_tri_intersect3D(double *C1, double *P1, double *P2,
	double *D1, double *Q1, double *Q2)
{
	double  t[3], p1[3], p2[3], r[3], r4[3];
	double beta1, beta2, beta3;
	double gama1, gama2, gama3;
	double det1, det2, det3;
	double dp0, dp1, dp2;
	double dq1, dq2, dq3, dr, dr3;
	double alpha1, alpha2;
	bool alpha1_legal, alpha2_legal;
	double  SF;
	bool beta1_legal, beta2_legal;

	myVmV(r, D1, C1);
	// determinant computation	
	dp0 = P1[1] * P2[2] - P2[1] * P1[2];
	dp1 = P1[0] * P2[2] - P2[0] * P1[2];
	dp2 = P1[0] * P2[1] - P2[0] * P1[1];
	dq1 = Q1[0] * dp0 - Q1[1] * dp1 + Q1[2] * dp2;
	dq2 = Q2[0] * dp0 - Q2[1] * dp1 + Q2[2] * dp2;
	dr = -r[0] * dp0 + r[1] * dp1 - r[2] * dp2;



	beta1 = dr * dq2;  // beta1, beta2 are scaled so that beta_i=beta_i*dq1*dq2
	beta2 = dr * dq1;
	beta1_legal = (beta2 >= 0) && (beta2 <= dq1 * dq1) && (dq1 != 0);
	beta2_legal = (beta1 >= 0) && (beta1 <= dq2 * dq2) && (dq2 != 0);

	dq3 = dq2 - dq1;
	dr3 = +dr - dq1;   // actually this is -dr3


	if ((dq1 == 0) && (dq2 == 0))
	{
		if (dr != 0) return 0;  // triangles are on parallel planes
		else
		{						// triangles are on the same plane
			double C2[3], C3[3], D2[3], D3[3], N1[3];
			// We use the coplanar test of Moller which takes the 6 vertices and 2 normals  
			//as input.
			myVpV(C2, C1, P1);
			myVpV(C3, C1, P2);
			myVpV(D2, D1, Q1);
			myVpV(D3, D1, Q2);
			CROSS(N1, P1, P2);
			return coplanar_tri_tri(N1, C1, C2, C3, D1, D2, D3);
		}
	}

	else if (!beta2_legal && !beta1_legal) return 0;// fast reject-all vertices are on
													// the same side of the triangle plane

	else if (beta2_legal && beta1_legal)    //beta1, beta2
	{
		SF = dq1 * dq2;
		sVpsV_2(t, beta2, Q2, (-beta1), Q1);
	}

	else if (beta1_legal && !beta2_legal)   //beta1, beta3
	{
		SF = dq1 * dq3;
		beta1 = beta1 - beta2;   // all betas are multiplied by a positive SF
		beta3 = dr3 * dq1;
		sVpsV_2(t, (SF - beta3 - beta1), Q1, beta3, Q2);
	}

	else if (beta2_legal && !beta1_legal) //beta2, beta3
	{
		SF = dq2 * dq3;
		beta2 = beta1 - beta2;   // all betas are multiplied by a positive SF
		beta3 = dr3 * dq2;
		sVpsV_2(t, (SF - beta3), Q1, (beta3 - beta2), Q2);
		Q1 = Q2;
		beta1 = beta2;
	}
	sVpsV_2(r4, SF, r, beta1, Q1);
	seg_collide3(t, r4);  // calculates the 2D intersection
	return 0;
}

int coplanar_tri_tri(double N[3], double V0[3], double V1[3], double V2[3], double U0[3], double U1[3], double U2[3])
{
	return 0;
}
