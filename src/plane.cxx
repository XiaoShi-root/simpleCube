#include <plane.h>

colors direction2color(directions d){
    switch (d)
    {
    case left:
        return red;
        break;
    case right:
        return orange;
        break;
    case up:
        return yellow;
        break;
    case down:
        return white;
        break;
    case front:
        return blue;
        break;
    case back:
        return green;
    default:
        return white;
        break;
    }
}
plane::plane(glm::vec3 o,directions d, Shader *s){
    color = direction2color(d);
    shader = s;
    p1 = o;
    glm::vec3 v1,v2;
    switch (d)
    {
    case left:
    case right:
        v1 = glm::vec3(0.0,0.0,-1.0);
        v2 = glm::vec3(0.0,1.0,0.0);
        break;
    case up:
    case down:
        v1 = glm::vec3(0.0,0.0,-1.0);
        v2 = glm::vec3(1.0,0.0,0.0);
        break;
    case front:
    case back:
        v1 = glm::vec3(0.0,1.0,0.0);
        v2 = glm::vec3(1.0,0.0,0.0);
        break;
    default:
        break;
    }
    p2 = o+v1;
    p3 = o+v2;
    p4 = o+v1+v2;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    unsigned int indeces[] = {
        0,1,3,
        0,2,3
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indeces),indeces,GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER,4*4*sizeof(float),(void *)0,GL_STREAM_DRAW );
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
};
void plane::use_program(){
    shader->use();
}
void plane::render(){
    use_program();
    float points[]={
        p1.x,p1.y,p1.z,(float)color,
        p2.x,p2.y,p2.z,(float)color,
        p3.x,p3.y,p3.z,(float)color,
        p4.x,p4.y,p4.z,(float)color,
    };
    // unsigned int mat_loc= glGetUniformLocation(shader->ID,"transform");
    // glm::mat4 transform = glm::mat4(1.0);
    // glUniformMatrix4fv(mat_loc,1,GL_FALSE,glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STREAM_DRAW);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void *)0);
    return;
};

void plane::rotate(axis a,float deg){
    transform = glm::mat4(1.0);
    glm::vec3 axis_vec;
    switch (a)
    {
    case x:
        axis_vec = glm::vec3(1.0,0.0,0.0);
        break;
    case y:
        axis_vec = glm::vec3(0.0,1.0,0.0);

    case z:
        axis_vec = glm::vec3(0.0,0.0,1.0);
    default:
        break;
    }
    transform = glm::rotate(transform,glm::radians(deg),axis_vec);
    shader->setMat4("transform",transform);
}

void plane::rotate(axis a,float deg,glm::vec3 o){
        transform = glm::mat4(1.0);
    glm::vec3 axis_vec;
    switch (a)
    {
    case x:
        axis_vec = glm::vec3(1.0,0.0,0.0);
        break;
    case y:
        axis_vec = glm::vec3(0.0,1.0,0.0);

    case z:
        axis_vec = glm::vec3(0.0,0.0,1.0);
    default:
        break;
    }
    transform = glm::translate(transform,-o);
    transform = glm::rotate(transform,glm::radians(deg),axis_vec);
    transform = glm::translate(transform,o);
    shader->setMat4("transform",transform);
}