#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;


#define SQRT2	1.41421356237
#define SQRT3	1.73205080757

#define SIZE 		15
#define HASH_MAGNITUDE	(6.0 / (SQRT2 + 1.0) - 1.0) // Perfect if: HASH_MAGNITUDE < 2 * (KERNEL + 1) / (SQRT2 + 1) - 1
#define KERNEL		2 // Perfect if: KERNEL >= floor ((HASH_MAGNITUDE + 1) * (SQRT2 + 1) / 2)

#define BORDER
//#define CENTER
#define DISTANCE
#define STRIPS
#define HEXAGONAL

float hash (in int index) {
	float x = float (index);
	return HASH_MAGNITUDE * 0.5 * sin (sin (x) * x + sin (x * x) * time);
}

vec2 pointInCell (in ivec2 cell) {
	int index = cell.x + cell.y * SIZE;
	vec2 point = vec2 (cell);
	#ifdef HEXAGONAL
	point.x += fract (point.y * 0.5) - 0.25;
	#endif
	return point + vec2 (hash (index), hash (index + 1)) * (0.5 + 0.5 * sin (time * 0.5));
}

void main () {
//	vec2 p = float (SIZE) * (gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;

	vec3 r = reflect( e, n );
        r = e - 2. * dot( n, e ) * n;
        float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
        vec2 p = float (SIZE) *r.xy / m + .5;


	#ifdef HEXAGONAL
	p.y /= SQRT3 * 0.5;
	#endif
	ivec2 pCell = ivec2 (floor (p + 0.5));

	float dMin = HASH_MAGNITUDE + 1.0;
	vec2 pqMin;
	ivec2 minCell;
	for (int y = -KERNEL; y <= KERNEL; ++y) {
		for (int x = -KERNEL; x <= KERNEL; ++x) {
			ivec2 qCell = pCell + ivec2 (x, y);
			vec2 pq = pointInCell (qCell) - p;
			#ifdef HEXAGONAL
			pq.y *= SQRT3 * 0.5;
			#endif
			float d = dot (pq, pq);
			if (d < dMin) {
				dMin = d;
				pqMin = pq;
				minCell = qCell;
			}
		}
	}
	int col = minCell.x + minCell.y * SIZE;
	vec4 color = 0.6 + vec4 (hash (col), hash (col + 1), hash (col + 2), 0.0) * 0.8 / HASH_MAGNITUDE;

	#ifdef CENTER
	dMin = sqrt (dMin);
	#else
	dMin = HASH_MAGNITUDE + 1.0;
	#endif

	#ifdef BORDER
	for (int y = -KERNEL; y <= KERNEL; ++y) {
		for (int x = -KERNEL; x <= KERNEL; ++x) {
			ivec2 qCell = pCell + ivec2 (x, y);
			if (qCell != minCell) {
				vec2 pq = pointInCell (qCell) - p;
				#ifdef HEXAGONAL
				pq.y *= SQRT3 * 0.5;
				#endif
				dMin = min (dMin, dot (0.5 * (pqMin + pq), normalize (pq - pqMin)));
			}
		}
	}
	#endif

	color *= smoothstep (0.02, 0.1, dMin);

	#ifdef DISTANCE
	color *= 0.9 + 0.1 * sin (dMin * 40.0);
	#endif

	#ifdef STRIPS
	float strip = float (col);
	float stripCos = cos (strip);
	float stripSin = sin (strip);
	strip = mix (1.0, sin (40.0 * (pqMin.x * stripCos - pqMin.y * stripSin)), mod (strip * 0.5, 2.0));
	strip *= sin (40.0 * (pqMin.x * stripSin + pqMin.y * stripCos));
	color *= 0.8 + 0.2 * strip;
	#endif

	gl_FragColor = color;
}
