#include "../include/plane.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


colors direction2color(directions d){
	switch (d)
	{
	case up:
		return yellow;
		break;
	case down:
		return white;
		break;
	case left:
		return red;
		break;
	case right:
		return orange;
		break;
	case front:
		return blue;
		break;
	case back:
		return green;
		break;
	default:
	case none:
		return grey;
		break;
	}
}

void init_vector(plane *p,directions d){
    switch (d)
    {
    case left:
    case right:
        p->v1[z] = -1.0f;
        p->v2[y] = 1.0f;
        break;
    case up:
    case down:
        p->v1[z] = -1.0f;
        p->v2[x] = 1.0f;
        break;
    case front:
    case back:
        p->v1[y] = 1.0f;
        p->v2[x] = 1.0f;
    default:
        break;
    }
    return;
}

plane *create_plane(float o[3],directions d,GLint program){
    plane *p = (plane *)calloc(1,sizeof(plane));
    p->origin[0] = o[0];
    p->origin[1] = o[1];
    p->origin[2] = o[2];
    p->program = program;
    GLuint indices[] = {
        0,2,3,
        0,1,3
    };
    unsigned int VAO,VBO,EBO;

    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&EBO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,64L,(void*)0,GL_STREAM_DRAW);                  //I don't know why i should do like this.
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void *)0);
    printf("%d\n",glGetError()); 
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    p->VAO = VAO;
    p->VBO = VBO;
    p->EBO = EBO;
    init_vector(p,d);

    return p;
}
void use_program(plane *p){
    glUseProgram(p->program);
}
int time;
void render_plane(plane *p){
    use_program(p);
    float *v1 = p->origin;
    float *v2 = add(p->origin,p->v1);
    float *v3 = add(p->origin,p->v2);
    float *temp = add(p->v1,p->v2);
    float *v4 = add(p->origin,temp);
    free(temp);
    float points[] ={
        v1[0],v1[1],v1[2],
        (float)p->color,
        v2[0],v2[1],v2[2],
        (float)p->color,
        v3[0],v3[1],v3[2],
        (float)p->color,
        v4[0],v4[1],v4[2],
        (float)p->color,
    } ;
    free(v2);
    free(v3);
    free(v4);
    glBindVertexArray(p->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,p->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,p->EBO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
}