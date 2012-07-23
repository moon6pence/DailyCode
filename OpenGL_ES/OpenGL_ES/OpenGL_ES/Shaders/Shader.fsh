//
//  Shader.fsh
//  OpenGL_ES
//
//  Created by khkim on 12. 7. 23..
//  Copyright (c) 2012년 chickenjuice.net. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
