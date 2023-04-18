uniform sampler2D u_texture;
varying highp vec2 v_textcoord;
uniform bool u_drawMode;

// https://habr.com/ru/post/133828/
// Определим в ней два вектора c0 содержащий белый цвет (цвет представляется как, rgba)
// и c1 — красный цвет, а дальше присваиваем выходному вектору gl_FragColor значение,
// полученное для каждого пикселя с помощью функции mix — функции линейной интерполяции между двумя значениями:
// mix (vec4 x, vec4 y, float a) — выражается формулой: x * ( 1.0 - a )+y * a
// Изменяющимся параметром a здесь будет значение .y текстурного вектора, соответствующее векторной коордитнате по оси y.

void main(void)
{

    lowp vec4 c0 = vec4( 1.0, 1.0, 1.0, 1.0 );

    lowp vec4 c1 = vec4( 0.5, 0.5, 0.5, 1.0 );

    if(u_drawMode == true)
        gl_FragColor = mix( c0, c1, v_textcoord.y );
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
