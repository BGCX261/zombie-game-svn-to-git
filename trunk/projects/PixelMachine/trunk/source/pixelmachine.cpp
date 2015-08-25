/*
** PixelMachine
** v 0.1.20070220
**
** A ray-traced 3D renderer by SuperJer
**
** Email = superjer@superjer.com
** Web   = http://www.superjer.com/
**
** You can do whatever you like with this code, I don't care,
** just please recognize me as the original author. :)
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <time.h>

#include <cassert>

#include "collision.hpp"


const int TERDIV =  8;    // terrain divisions (powers of 2 ONLY!)
const int TERSECS = 8;    // sections per division
// this will work out to (TERDIV*TERSECS+1)^3 cubes!!

const int BSIZE = (TERDIV*TERSECS+1);
const double SQSIZE = (1000.0/(double)BSIZE);
const int MULTIS = 2;     // level of multisampling (makes rendering n^2 times slower!!)

const int W = 200;        // output image size
const int H = 150;
const int FRAMES = 1;     // frames of animation

const int XY = 0;
const int XZ = 1;
const int YZ = 2;

const double SCATTER = 0.00003;

const int SUNS = 2;

const int SPHERES = 100;

double terrain[BSIZE][BSIZE];

unsigned char *img = NULL;
int w;
int h;
int multis;
int frames;
COLOR blocks[BSIZE][BSIZE][BSIZE] = {0};

double blocksize = 1000.0/((double)BSIZE);

Vector cam;
Vector tar;
double tarw;
double pixpitch;
double subpitch;

Vector sun[SUNS];
SPHERE sphere[SPHERES];


void savebmp( const char *filename, int w, int h );
void preview( int w, int h );
void render( int w, int h, bool quiet );
void raytrace( COLOR &color, const Vector &cam, const Vector &ray, bool lighttest );

int main(int argc,char **argv)
{
	int i,j,k;
	int w,h;
	char str[80];
	unsigned seed = (unsigned)-1;

	w = W;
	h = H;
	multis = MULTIS;
	frames = FRAMES;

	for(i=1; i<argc; i++)
	{
		if( argv[i][0]=='-' ) switch( argv[i][1] )
		{
				case 'w': w = atoi(argv[i]+2); break;
				case 'h': h = atoi(argv[i]+2); break;
				case 'm': multis = atoi(argv[i]+2); break;
				case 'v': frames = atoi(argv[i]+2); break;
				case '-':
					if( !strcmp(argv[i],"--help") )
					{
						printf( "Usage: [OPTION]... [SEED]\nRender ray-traced 3D images generated randomly with seed number SEED.\n\n  option default description\n  -wNUM  %7d Set image output width to NUM\n  -hNUM  %7d Set image output height to NUM\n  -mNUM  %7d Set multisampling level to NUM (level 2 recommended)\n  -vNUM  %7d Render a sequence of NUM images for a video\n",W,H,MULTIS,FRAMES );
						exit(0);
					}
				default:
					fprintf( stderr, "Halt! -%c isn't one of my options!\n", argv[i][1] );
					exit(-1);
		}
		else if( seed==(unsigned)-1 )
		{
			seed = atoi(argv[i]);
			if( !seed )
			{
				fprintf( stderr, "Halt! SEED ought to be a positive number, not %s\n", argv[i] );
				exit(-1);
			}
		}
		else
		{
			fprintf( stderr, "Halt! I'm confused by cmd line argument #%d: %s\n", i, argv[i] );
			exit(-1);
		}
	}

	if( seed==(unsigned)-1 )
	{
		seed = (unsigned)time(NULL);
	}
	//seed = 1171997907;	// this one is pretty good!
	srand(seed);

	FILE *f;
	f = fopen("seed.txt","w");
	fprintf(f,"%d",seed);
	fclose(f);

	//generate terrain
	printf("Generating...\n");
	int div = TERDIV;
	for(i=0;i<BSIZE;i+=div)
	{
		for(j=0;j<BSIZE;j+=div)
		{
			terrain[i][j] = (double)(rand()%1000)*0.5-200.0;
		}
	}

	while( div>1 )
	{
		for(i=div/2;i<BSIZE;i+=div)
		{
			for(j=div/2;j<BSIZE;j+=div)
			{
				terrain[i][j] = (terrain[i-div/2][j-div/2] + terrain[i-div/2][j+div/2] + terrain[i+div/2][j-div/2] + terrain[i+div/2][j+div/2])*0.25;
			}
		}

		for(i=0;i<BSIZE;i+=div/2)
		{
			for(j=0;j<BSIZE;j+=div/2)
			{
				if( (i+j)%div )
				{
					if( i==0 )
						terrain[i][j] = (                      terrain[i+div/2][j] + terrain[i][j-div/2] + terrain[i][j+div/2])*0.33333;
					else if( i==BSIZE-1 )
						terrain[i][j] = (terrain[i-div/2][j]                       + terrain[i][j-div/2] + terrain[i][j+div/2])*0.33333;
					else if( j==0 )
						terrain[i][j] = (terrain[i-div/2][j] + terrain[i+div/2][j]                       + terrain[i][j+div/2])*0.33333;
					else if( j==BSIZE-1 )
						terrain[i][j] = (terrain[i-div/2][j] + terrain[i+div/2][j] + terrain[i][j-div/2]                      )*0.33333;
					else
						terrain[i][j] = (terrain[i-div/2][j] + terrain[i+div/2][j] + terrain[i][j-div/2] + terrain[i][j+div/2])*0.25;
				}
			}
		}
		div /= 2;
	}


	// represent terrain as those stupid cubes (this is dumb I KNOW!)
	for(k=0; k<BSIZE; k++)
	{
		for(i=0; i<BSIZE; i++)
		{
			for(j=0; j<BSIZE; j++)
			{
				if( k==0 )
				{
					blocks[i][j][k].r = (double)(rand()%30)/255.0 + 0.50;
					blocks[i][j][k].g = (double)(rand()%40)/255.0 + 0.40;
					blocks[i][j][k].b = (double)(rand()%20)/255.0 + 0.05;
					blocks[i][j][k].a = 1.0; //(double)(rand()%100+1)*0.01;
				}
				else if( i>(BSIZE*2)/5 && i<(BSIZE*3)/5 && j>(BSIZE*2)/5 && j<(BSIZE*3)/5 ) // shiny area!
				{
					if( k<BSIZE/5 )
						blocks[i][j][k] = blocks[i][j][k-1];
					else
					{
						blocks[i][j][k].r = 0.0;
						blocks[i][j][k].g = 0.0;
						blocks[i][j][k].b = 0.0;
						blocks[i][j][k].a = 0.0;
					}
				}
				else if( terrain[i][j] >= k*(1000.0/(double)BSIZE ) )
				{
					blocks[i][j][k] = blocks[i][j][k-1];
				}
				else
				{
					blocks[i][j][k].r = 0.0;
					blocks[i][j][k].g = 0.0;
					blocks[i][j][k].b = 0.0;
					blocks[i][j][k].a = 0.0;
				}


				sun[0].x = (double)(rand()%1500) - 250.0;
				sun[0].y = (double)(rand()%1500) - 250.0;
				sun[0].z = (double)(rand()%700) + 100.0;

				for(int sunIter=0; sunIter<SUNS; sunIter++)
				{
					sun[i].x = sun[0].x + ((double)i)*0.4;
					sun[i].y = sun[0].y + ((double)i)*0.3;
					sun[i].z = sun[0].z + ((double)i)*0.2;
				}

				cam.x = (double)(rand()%900) + 50.0;
				cam.y = (double)(rand()%900) + 50.0;
				cam.z = terrain[int(cam.x/SQSIZE)][int(cam.y/SQSIZE)] + (double)(rand()%150) + 50.0;

				tar.x = (double)(rand()%900) + 50.0;
				tar.y = (double)(rand()%900) + 50.0;
				tar.z = cam.z - (double)(rand()%400);

				for(int sphereIter =0; sphereIter < SPHERES; sphereIter++)
				{
					sphere[i].center.x = (double)(rand()%10000)*0.1;
					sphere[i].center.y = (double)(rand()%10000)*0.1;
					sphere[i].center.z = (double)(rand()%3500)*0.1;
					sphere[i].radius = (double)(rand()%10000)*0.005;
					sphere[i].color.a = 1.0;
					sphere[i].color.r = (double)(rand()%255)/255.0;
					sphere[i].color.g = (double)(rand()%255)/255.0;
					sphere[i].color.b = (double)(rand()%255)/255.0;
				}

				sphere[0].center.x = (double)(BSIZE*SQSIZE/2);
				sphere[0].center.y = (double)(BSIZE*SQSIZE/2);
				sphere[0].center.z = (double)(40*SQSIZE);
				sphere[0].radius = 30.0;

				Vector lin;
				subtract( lin, tar, cam );
				normalize( lin, 2.0 );

				printf("Rendering...\n");
				for(int frameIter=0;frameIter<frames;frameIter++)
				{
					render(61,22,true);
					preview(61,22);

					/**/
					render(w,h,false);

					printf("Writing frame %.4d ...\n",i);
					if( frames > 1 )
						sprintf(str,"./video/frame%.4d.bmp",i);
					else
						sprintf(str,"./img.bmp");
					savebmp(str,w,h);
					/**/    

					subtract( cam, cam, lin );
					tar.z -= 0.85;
					cam.z -= 0.9;

					for(int sphereIter=0;sphereIter<SPHERES;sphereIter++)
					{
						switch( j%3 )
						{
						case 2: sphere[j].center.z += 1.1; break;
						case 1: sphere[j].center.z -= 1.1; break;
						case 0: break;;
						}
					}
				}

				free(img);
			}
		}
	}
}

void savebmp( const char *filename, int w, int h )
{
	int i;
	FILE *f;
	int filesize = 54 + 3*w*h;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};

	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);

	//f = fopen("img.raw","wb");
	//fwrite(img,3,w*h,f);
	//fclose(f);

	f = fopen(filename,"wb");
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	for(i=0; i<h; i++)
	{
		fwrite(img+(w*(h-i-1)*3),3,w,f);
		fwrite(bmppad,1,(4-(w*3)%4)%4,f);
	}
	fclose(f);
}




void preview( int w, int h )
{
	int i,j;
	int brightness,min,max,step;

	max = 0;
	min = 768;

	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			brightness = img[(i+j*w)*3+0] + img[(i+j*w)*3+1] + img[(i+j*w)*3+2];
			if( brightness>max ){ max = brightness; }
			if( brightness<min ){ min = brightness; }
		}
	}

	step = (max-min)/11;

	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			brightness = img[(i+j*w)*3+0] + img[(i+j*w)*3+1] + img[(i+j*w)*3+2];
			if( brightness>min+step*10 )
			{
				printf("#");
			}
			else if( brightness>min+step*9 )
			{
				printf("@");
			}
			else if( brightness>min+step*8 )
			{
				printf("8");
			}
			else if( brightness>min+step*7 )
			{
				printf("$");
			}
			else if( brightness>min+step*6 )
			{
				printf("?");
			}
			else if( brightness>min+step*5 )
			{
				printf("%%");
			}
			else if( brightness>min+step*4 )
			{
				printf("*");
			}
			else if( brightness>min+step*3 )
			{
				printf(";");
			}
			else if( brightness>min+step*2 )
			{
				printf(",");
			}
			else if( brightness>min+step*1 )
			{
				printf("`");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}



void render( int w, int h, bool quiet )
{
	int i,j,k,m,n;

	COLOR *color;
	color = (COLOR*)malloc(sizeof(COLOR)*(multis*multis));

	if( img )
	{
		free( img );
	}

	img = (unsigned char *)malloc(3*w*h);
	memset(img,0,sizeof(img));

	subtract( tar, tar, cam );
	normalize( tar, 1.0 );
	add( tar, tar, cam );

	tarw = 2.0;
	pixpitch = tarw/w;
	subpitch = pixpitch/((double)multis+1.0);

	Vector lin;
	lin.x = tar.x - cam.x;
	lin.y = tar.y - cam.y;
	lin.z = tar.z - cam.z;

	double di,dj;
	double r,g,b;
	int subgoal;

	subgoal = 0;

	for(i=0; i<w; i++)
	{
		for(j=0; j<h; j++)
		{
			di = (double)i;
			dj = (double)j;

			for(m=0; m<multis; m++)
			{
				for(n=0; n<multis; n++)
				{
					k = m + n*multis;

					Vector wing;
					wing.x =  lin.y;
					wing.y = -lin.x;
					wing.z = 0.0;

					Vector head;
					cross( head, lin, wing );

					normalize( wing, (di-(double)w/2.0)*pixpitch + subpitch*m );
					normalize( head, (dj-(double)h/2.0)*pixpitch + subpitch*n );

					Vector ray;
					ray.x = tar.x+wing.x+head.x - cam.x;
					ray.y = tar.y+wing.y+head.y - cam.y;
					ray.z = tar.z+wing.z+head.z - cam.z;

					raytrace( color[k], cam, ray, false );
				}
			}

			r = 0.0;
			g = 0.0;
			b = 0.0;

			for(k=0;k<(multis*multis);k++)
			{
				r += color[k].r;
				g += color[k].g;
				b += color[k].b;
			}

			r /= (double)(multis*multis);
			g /= (double)(multis*multis);
			b /= (double)(multis*multis);

			img[(i+j*w)*3+2] = (unsigned char)(255.0*r+0.5);
			img[(i+j*w)*3+1] = (unsigned char)(255.0*g+0.5);
			img[(i+j*w)*3+0] = (unsigned char)(255.0*b+0.5);

			//printf( "%3.3f %3.3f %3.3f\n", r, g, b );
			//getch();
		}

		if( !quiet )
		{
			if( (int)(1000.0*(double)i/(double)w) >= subgoal )
			{
				subgoal += 20;
				if( subgoal%100==0 )
				{
					printf("%d",subgoal/10);
				}
				else
				{
					printf(".");
				}
				fflush(stdout);
			}
		}
	}
	if( !quiet )
	{
		printf("\n");
	}
}




void raytrace( COLOR &color, const Vector &cam, const Vector &ray, bool lighttest )
{
	int i,k;
	double r,g,b,a;
	int blockx,blocky,blockz;
	Vector hit0,hit1,hit2;
	Vector hit;
	double p0,p1,p2;
	double p,potp;
	int side;
	int hitobject,pothitobject;

	side = XY;

	r = 0.0;
	g = 0.0;
	b = 0.0;
	a = 0.0;

	// spheres
	potp = 99999999.0;
	hitobject = -1;
	pothitobject = -1;
	for(i=0; i<SPHERES; i++)
	{
		Vector v;
		v = ray;
		CollideRaySphere(p0,cam,v,sphere[i].center,sphere[i].radius);

		if(p0!=NO_COLLISION && p0+0.00001 < potp)
		{
			pothitobject = i;
			potp = p0+0.00001;
		}
	}


	p = 0.0;
	k = 0;
	while( 1 )
	{
		// find current space block
		blockx = (int)((cam.x+p*ray.x)/blocksize);
		blocky = (int)((cam.y+p*ray.y)/blocksize);
		blockz = (int)((cam.z+p*ray.z)/blocksize);

		if( blockx<1 || blockx>BSIZE-1 || blocky<1 || blocky>BSIZE-1 || blockz<0 || blockz>BSIZE-1 || k>1000 || (lighttest && p>=1.0) )
		{
			if( !lighttest )
			{
				double dot = (ray.x*ray.x + ray.y*ray.y);
				double div = sqrt(dot) * sqrt(ray.x*ray.x + ray.y*ray.y + ray.z*ray.z);
				assert(div != 0.0);
				double ang = fabs(dot/div);
				ang *= ang;
				r = r + 0.5*ang*(1.0-a);
				g = g + 0.6*ang*(1.0-a);
				b = b + 1.0*ang*(1.0-a);
				a = 1.0;
			}
			break;
		}

		k++;

		// pickup color from current space block
		COLOR *c;
		if( hitobject>=0 )
		{
			c = &sphere[hitobject].color;
		}
		else
		{
			c = &blocks[blockx][blocky][blockz];
		}

		if( k>0 && (c->a>0.0 || hitobject>=0 ) )
		{
			if( lighttest )
			{
				color.r = 0.0;
				color.g = 0.0;
				color.b = 0.0;
				color.a = 0.7;
				return;
			}

			// get incoming light/shadow
			Vector beam;
			COLOR light;
			COLOR finallight;

			finallight.r = 0.0;
			finallight.g = 0.0;
			finallight.b = 0.0;

			for(int m=0;m<SUNS;m++)
			{
				subtract( beam, sun[m], hit );
				raytrace( light, hit, beam, true );

				double falloff = 800000.0/(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
				light.r = light.r*0.0+(1.0-light.a)*falloff;
				light.g = light.g*0.0+(1.0-light.a)*falloff;
				light.b = light.b*0.0+(1.0-light.a)*falloff;

				finallight.r += light.r;
				finallight.g += light.g;
				finallight.b += light.b;
			}

			finallight.r /= (double)SUNS;
			finallight.g /= (double)SUNS;
			finallight.b /= (double)SUNS;

			double sidefactor = (side==XY?1.0:(side==XZ?0.8:0.6));

			if( blockz==0 || hitobject>=0 || blockx>(BSIZE*2)/5 && blockx<(BSIZE*3)/5 && blocky>(BSIZE*2)/5 && blocky<(BSIZE*3)/5 ) //reflect!
			{
				Vector ray2 = ray;

				if( hitobject>=0 )
				{
					Vector rad;

					subtract( rad, hit, sphere[hitobject].center );
					mirror( ray2, ray, rad );
					ray2.x = -ray2.x;
					ray2.y = -ray2.y;
					ray2.z = -ray2.z;
				}
				else if( side==XY )
				{
					ray2.z = -ray.z;
				}
				else if( side==XZ )
				{
					ray2.y = -ray.y;
				}
				else 
				{
					ray2.x = -ray.x;
				}

				//printf("%f",sin( (hit.z)*0.01 )*0.1 );

				double scatter;
				if( hitobject>=0 )
				{
					scatter = 0.0;
				}
				else
				{
					scatter = SCATTER;
				}

				ray2.x *= 1.0 + (double)(rand()%1000)*scatter; // + sin( (hit.z)*0.1 )*0.3;
				ray2.y *= 1.0 + (double)(rand()%1000)*scatter; // + cos( (hit.z)*0.1 )*0.3;
				ray2.z *= 1.0 + (double)(rand()%1000)*scatter; // + sin( (hit.x)*0.1 )*0.3;

				raytrace(color, hit, ray2, false);

				r = r + color.r*(1.0-a)*sidefactor*finallight.r;
				g = g + color.g*(1.0-a)*sidefactor*finallight.g;
				b = b + color.b*(1.0-a)*sidefactor*finallight.b;
				a = 1.0;

				break;
			}

			r = r + c->r*c->a*(1.0-a)*sidefactor*finallight.r;
			g = g + c->g*c->a*(1.0-a)*sidefactor*finallight.g;
			b = b + c->b*c->a*(1.0-a)*sidefactor*finallight.b;
			a += c->a*(1.0-a);

			if( a>=0.98 )
			{
				a = 1.0;
				break;
			}
		}

		// move to next space block
		hit0.x = (double)blockx*blocksize + (ray.x>0.0?blocksize:0.0);
		hit1.y = (double)blocky*blocksize + (ray.y>0.0?blocksize:0.0);
		hit2.z = (double)blockz*blocksize + (ray.z>0.0?blocksize:0.0);

		p0 = (hit0.x-cam.x)/(ray.x) + 0.00001;
		p1 = (hit1.y-cam.y)/(ray.y) + 0.00001;
		p2 = (hit2.z-cam.z)/(ray.z) + 0.00001;

		if( (int)((cam.x+p0*ray.x)/blocksize)==blockx && (int)((cam.y+p0*ray.y)/blocksize)==blocky && (int)((cam.z+p0*ray.z)/blocksize)==blockz )
		{
			p0 = 99999.0;

			if( (int)((cam.x+p1*ray.x)/blocksize)==blockx && (int)((cam.y+p1*ray.y)/blocksize)==blocky && (int)((cam.z+p1*ray.z)/blocksize)==blockz )
			{
				p1 = 99999.0;
			}

			if( (int)((cam.x+p2*ray.x)/blocksize)==blockx && (int)((cam.y+p2*ray.y)/blocksize)==blocky && (int)((cam.z+p2*ray.z)/blocksize)==blockz )
			{
				p2 = 99999.0;
			}

			if( p0<p || p1<p || p2<p ){ break; }

			if( p0<p1 && p0<p2 )
			{
				side = YZ;
				p = p0;
			}
			else if( p1<p2 )
			{
				side = XZ;
				p = p1;
			}
			else
			{
				side = XY;
				p = p2;
			}

			if( p>potp )
			{
				hitobject = pothitobject;
				side = XY;
				p = potp;
			}

			hit.x = (p-0.00002)*ray.x + cam.x;
			hit.y = (p-0.00002)*ray.y + cam.y;
			hit.z = (p-0.00002)*ray.z + cam.z;

			//printf( "%.4d %.4d %.4d\n", blockx, blocky, blockz );
		}

		if( r>1.0 ){ r = 1.0; }
		if( g>1.0 ){ g = 1.0; }
		if( b>1.0 ){ b = 1.0; }
		if( r<0.0 ){ r = 0.0; }
		if( g<0.0 ){ g = 0.0; }
		if( b<0.0 ){ b = 0.0; }

		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
}
