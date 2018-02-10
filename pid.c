#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

float op, oi, od, oa;
float roi, rod, rop, roa;
float p, i, d;
float pe, ie, de, de1, de2, de3;
int k1, k2, k3;
                FILE *fp;

        float dt=0.1;
        float pbest=100;
        float ibest=5;
        float dbest=0.01;

      long s = 40000;
      long oimax = 4000000;
      long oimin = -10000;
      long odmin = -100000;
      long odmax = 100000;
      long oamax = 5000000;
      long oamin = 10;

      long temp4=40000;
      long temp3=40000;
      long temp2=40000;
      long temp1=40000;
      long temp0=40000;
      long tmp;
   
int intconstrain(float var, float max, float min)
        {
        if( var>=(int)max )
                var=(int)max;
        if( var<=(int)min )
                var=(int)min;
        else
                var=(int)var;
        return var;
         }

float constrain(float var, float max, float min)
        { 
                if( var>=max )
                        var=max;
                if( var<=min )
                        var=min;
        return var;
         }

float integral(float i, float s, float dt, int k1)
        {
                ie=dt * k1 * ( temp0 + temp1 - ( 2 * s ) ) / 2;
                oi = roi + ( ie * i );
                roi=constrain(oi, oimax, oimin);
        return roi;
         }


float derivative(float d, float s, float dt, int k1, int k2, int k3)
        { 

//              de = ( temp0 - temp1 ) / ( dt );

                de = ( temp0 - temp1 ) / ( dt*k1 );
//              de1 = ( temp0 - temp1 ) / ( dt*k1 );
//              de2 = ( temp0 - temp2 ) / ( dt*( k1+k2 ) );
//              de3 = ( temp0 - temp3 ) / ( dt*( k1+k2+k3 ) );
//              de=de1+de2+de3;

                od = d * de;
                rod=constrain(od, odmax, odmin);
/*
                if(od==-rod)
                d=d*0.1;
                if( od==odmax || od==odmin )
                { 
                rod=( rod+od )/2;
                d=d*0.1;
                }
                else
                rod=od;
*/
        return rod;
        }

float proportional(float p, float s)
        { 
                pe=temp0 - s;
                rop = p * pe;
        return rop;
         }

float pid(float p, float i, float d, float dt, float rop, float roi, float rod, float s, int k1, int k2, int k3)
        { 
                roi=integral(i, s, dt, k1);
                rop=proportional(p, s);
                rod=derivative(d, s, dt, k1, k2, k3);
                oa = rop + roi + rod + oamin;
                roa=intconstrain(( oa + 0.5 ), oamax, oamin);
        return roa;
         }



int main()
{ 
roa=150;
roi=1;
  oi=0;
  dt=dt / ( roa * 100 );
//1.12653 0.304294 -0.278493
  k1=1;
  k2=1;
  k3=1;
  p=pbest;
  i=ibest/( dt * k1 ); //should make these invariant
  d=dbest*dt*k1;
while( 1==1 ) 
        {  //while loop

//         fp = fopen("/sys/class/hwmon/hwmon3/temp1_input", "r");
//         fp = fopen("/sys/devices/virtual/thermal/thermal_zone11/temp", "r");
           fscanf(fp, "%ld", &tmp);
           fclose(fp);
                usleep(( dt * 1000000));
                dt=roa/1000000;
        if(tmp!=temp0)
        { 
                k3=k2;
                k2=k1;
                k1=1;
                temp4 = temp3;
                temp3 = temp2;
                temp2 = temp1;
                temp1 = temp0;
                temp0 = tmp;
        roa=pid(p, i, d, dt, rop, roi, rod, s, k1, k2, k3);
        dt=roa/1000000;
         }
        else
        { 
        k1=k1+1;
//      roa=pid(p, i, d, dt, rop, roi, rod, s, k1, k2, k3);
        roi=roi + ( i * dt * ( temp0 - s )  * 0.5 );
        roa=intconstrain( ( roa + roi ), oamax, oamin);
         }
/*
         if(roa>=oamax)
         dt=constrain( (dt*1.2), 5, 0.01);
         if(roa<=oamin)
         dt=constrain( (dt*0.9), 5, 0.01);

*/
  i=k1*ibest/dt; //should make these invariant
  d=dbest*dt*k1;

//      roa=pid(p, i, d, dt, rop, roi, rod, s, k1, k2, k3);
//      printf("temp %ld op %f oi %f od %f oa %ld \n", temp0, rop, roi, rod, (long)roa);
//      printf("temps %ld %ld %ld %ld %ld \n", temp0, temp1, temp2, temp3, temp4);
//      printf("k %d %d %d \n", k1, k2, k3);

/*         fp = fopen("/sys/devices/platform/it87.552/pwm1", "r+");
           fprintf(fp, "%ld", (long)roa);
           fclose(fp);

           fp = fopen("/sys/devices/platform/it87.552/pwm3", "r+");
           fprintf(fp, "%ld", (long)roa);
           fclose(fp);
*/
        } //end while y

return 0;
}

//
