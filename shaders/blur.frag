uniform sampler2D texture;
uniform float blur_radius;

void main()
{	
	vec4 pixel = vec4(0,0,0,0);
	
	float increment = blur_radius / 5.0;
	vec2 start = gl_TexCoord[0].xy - vec2( blur_radius, blur_radius );
	vec2 center = gl_TexCoord[0].xy;
	vec2 final = gl_TexCoord[0].xy + vec2( blur_radius, blur_radius );
	
	vec2 current = start;
	
	float total_weight = 0;
	float distance = 0;
	float x_dist = 0;
	float y_dist = 0;
	float weight = 0;
	
	while (current.x <= final.x)
	{
		current.y = start.y;
		while (current.y <= final.y)
		{
			/* find the distance between original coordinate and current coordinate */
			x_dist = current.x - center.x;
			y_dist = current.y - center.y;
			
			distance = sqrt( x_dist * x_dist + y_dist * y_dist );
			
			/* if the distance is greater than the radius, stop */
			if (distance > blur_radius)
			{
				current.y = current.y + increment;
				continue;
			}
			
			/* the weight of the pixel is gonna be larger the closer it is to the original coordinate */
			weight = 1.0 - (distance / blur_radius);
			
			/* add the weighted pixel */
			pixel = pixel + texture2D(texture, current) * weight;
			total_weight = total_weight + weight;
			
			current.y = current.y + increment;
		}
		current.x = current.x + increment;
	}
	
    gl_FragColor =  gl_Color * (pixel / total_weight);
}
