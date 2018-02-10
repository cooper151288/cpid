#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

      float op, oi, od, oa;
      float roi, rod, rop, roa;
      float p, i, d;
      float pe, ie, de, de1, de2, de3;
      FILE *fp;
      float dt=0.1;
      float pbest=100;
      float ibest=5;
      float dbest=0.01;

      long s = 40000;       //setpoint
      long oimax = 4000000; //max integral output
      long oimin = -10000;  //min integral output
      long odmin = -100000; //min derivative output
      long odmax = 100000;  //max derivative output
      long oamax = 5000000; //max total output
      long oamin = 10;      //min total output

      long temp4=40000;
      long temp3=40000;
      long temp2=40000;
      long temp1=40000;
      long temp0=40000; //historical temps
      long tmp;
   
int intconstrain(float var, float max, float min) //returns an integer between min and max
        {
        if( var>=(int)max )
                var=(int)max;
        if( var<=(int)min )
                var=(int)min;
        else
                var=(int)var;
        return var;
         }

float constrain(float var, float max, float min) //returns a float between min and max
        { 
                if( var>=max )
                        var=max;
                if( var<=min )
                        var=min;
        return var;
         }

float integral(float i, float s, float dt) //integral
        {
                ie=dt * ( temp0 + temp1 - ( 2 * s ) ) / 2; //added integral error
                oi = roi + ( ie * i );                          //adds to previous error
                roi=constrain(oi, oimax, oimin);                //constrain to acceptable values
        return roi;
         }


float derivative(float d, float dt)
        { 


                de = ( temp0 - temp1 ) / ( dt );  //derivative error
                od = d * de;                      //correction term
                rod=constrain(od, odmax, odmin);  //constrain

        return rod;
        }

float proportional(float p, float s)
        { 
                pe=temp0 - s; //prop. error
                rop = p * pe; //scale
        return rop;
         }

float pid(float p, float i, float d, float dt, float rop, float roi, float rod, float s, int k1, int k2, int k3)
        { 
                roi=integral(i, s, dt);
                rop=proportional(p, s);
                rod=derivative(d, dt);
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
  p=pbest;
  i=ibest/( dt ); //should make these invariant
  d=dbest*dt;
while( 1==1 ) 
        {  //while loop

//         fp = fopen("/sys/class/hwmon/hwmon3/temp1_input", "r");
//         fp = fopen("/sys/devices/virtual/thermal/thermal_zone11/temp", "r");
           fscanf(fp, "%ld", &tmp);
           fclose(fp);
                usleep(( dt * 1000000));
                dt=roa/1000000;
        if(tmp!=temp0)
        { ;
                temp4 = temp3;
                temp3 = temp2;
                temp2 = temp1;
                temp1 = temp0;
                temp0 = tmp;
        roa=pid(p, i, d, dt, rop, roi, rod, s);
        dt=roa/1000000;
         }
        else
        { 
//      roa=pid(p, i, d, dt, rop, roi, rod, s);
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
