#pragma once

#include <string_view>

constexpr std::string_view fragmentShader = R"(
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution; // Разрешение экрана в пикселях
uniform float u_time;       // Время в секундах

// === Новые параметры управления ===
uniform float u_slow;       // Замедление: 1.0 — нормальная скорость, 2.0 — в 2 раза медленнее, 5.0 — слоу-мо
uniform float u_rarity;     // Редкость: от 0.0 (сплошной ливень) до 0.95 (редкие единичные капли)

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

float rainLayer(vec2 uv, float scale, float speed, float slant, float seed, float time, float rarity) {
    uv.x += uv.y * slant;

    vec2 st = uv * vec2(scale * 1.6, scale * 0.15);
    st.y += time * speed; // Перемещение с учетом замедления

    vec2 id = floor(st) + seed;
    vec2 gv = fract(st) - vec2(0.5, 0.0);

    float rnd = hash(id);

    // Порог отсечения: чем выше rarity, тем меньше ячеек содержат каплю
    if (rnd < rarity) return 0.0;

    gv.x += (rnd - 0.5) * 0.6;

    float width = 0.05;
    float drop = smoothstep(width, 0.0, abs(gv.x));
    drop *= smoothstep(1.0, 0.1, gv.y) * smoothstep(0.0, 0.25, gv.y);

    return drop * (0.3 + 0.7 * rnd);
}

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.y;

    // Безопасный расчет замедления времени (защита от деления на 0)
    float time = u_time / max(u_slow, 0.001);

    // Ограничение редкости в пределах [0.0; 0.98]
    float rarity = clamp(u_rarity, 0.0, 0.98);

    float slant = 0.18;

    // Три слоя глубины
    float r1 = rainLayer(uv, 55.0, 16.0, slant, 1.0, time, rarity) * 0.25;
    float r2 = rainLayer(uv, 32.0, 24.0, slant, 2.0, time, rarity) * 0.45;
    float r3 = rainLayer(uv, 16.0, 36.0, slant, 3.0, time, rarity) * 0.75;

    float rain = clamp(r1 + r2 + r3, 0.0, 1.0);
    vec3 rainColor = vec3(0.85, 0.9, 0.98);

    gl_FragColor = vec4(rainColor, rain);
})";