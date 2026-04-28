#include "core/math/matrix/matrix.h"

void mat4_identity(mat4_t* mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat->m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void mat4_rotate_y(mat4_t* mat, f32 angle) {
    mat4_identity(mat);
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    mat->m[0][0] = c;
    mat->m[0][2] = s;
    mat->m[2][0] = -s;
    mat->m[2][2] = c;
}

void mat4_rotate_x(mat4_t* mat, f32 angle) {
    mat4_identity(mat);
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    mat->m[1][1] = c;
    mat->m[1][2] = -s;
    mat->m[2][1] = s;
    mat->m[2][2] = c;
}

void mat4_perspective(mat4_t* mat, f32 fov, f32 aspect, f32 near, f32 far) {
    mat4_identity(mat);
    f32 tan_half_fov = tanf(fov / 2.0f);

    mat->m[0][0] = 1.0f / (aspect * tan_half_fov);
    mat->m[1][1] = 1.0f / (tan_half_fov);   
    
    mat->m[2][2] = far / (far - near);
    mat->m[2][3] = 1.0f;            
    mat->m[3][2] = (-far * near) / (far - near);
    mat->m[3][3] = 0.0f;
}

void mat4_lookat(mat4_t* mat, vec3_t eye, vec3_t target, vec3_t up) {
    vec3_t z_axis = vec3_normalize(vec3_sub(target, eye));
    
    vec3_t x_axis = vec3_normalize(vec3_cross(up, z_axis));
    
    vec3_t y_axis = vec3_cross(z_axis, x_axis);

    mat->m[0][0] = x_axis.x;
    mat->m[0][1] = y_axis.x;
    mat->m[0][2] = z_axis.x;
    mat->m[0][3] = 0.0f;

    mat->m[1][0] = x_axis.y;
    mat->m[1][1] = y_axis.y;
    mat->m[1][2] = z_axis.y;
    mat->m[1][3] = 0.0f;

    mat->m[2][0] = x_axis.z;
    mat->m[2][1] = y_axis.z;
    mat->m[2][2] = z_axis.z;
    mat->m[2][3] = 0.0f;

    mat->m[3][0] = -vec3_dot(x_axis, eye);
    mat->m[3][1] = -vec3_dot(y_axis, eye);
    mat->m[3][2] = -vec3_dot(z_axis, eye);
    mat->m[3][3] = 1.0f;
}
