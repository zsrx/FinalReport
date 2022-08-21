/*
 * sfuntmpl_basic.c: Basic 'C' template for a level 2 S-function.
 *
 * Copyright 1990-2013 The MathWorks, Inc.
 */


/*
 * You must specify the S_FUNCTION_NAME as the name of your S-function
 * (i.e. replace sfuntmpl_basic with the name of your S-function).
 */

#define S_FUNCTION_NAME  shepwm_3
#define S_FUNCTION_LEVEL 2

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"
#include <math.h>


int state[4][4][4];
double RG;
int PA,PB,PC;
int A1,A2,A3,A4;
int B1,B2,B3,B4;
int C1,C2,C3,C4;
float PI=3.1415926535;

double sw_A_add;
double A1_temp;
double A2_temp;
double A3_temp;
double A4_temp;

double t;
double fsin;
int timeticker;
/* Error handling
 * --------------
 *
 * You should use the following technique to report errors encountered within
 * an S-function:
 *
 *       ssSetErrorStatus(S,"Error encountered due to ...");
 *       return;
 *
 * Note that the 2nd argument to ssSetErrorStatus must be persistent memory.
 * It cannot be a local variable. For example the following will cause
 * unpredictable errors:
 *
 *      mdlOutputs()
 *      {
 *         char msg[256];         {ILLEGAL: to fix use "static char msg[256];"}
 *         sprintf(msg,"Error due to %s", string);
 *         ssSetErrorStatus(S,msg);
 *         return;
 *      }
 *
 */

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortRequiredContiguous(S, 0, true); /*direct input signal access*/
    /*
     * Set direct feedthrough flag (1=yes, 0=no).
     * A port has direct feedthrough if the input is used in either
     * the mdlOutputs or mdlGetTimeOfNextVarHit functions.
     */
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 12)) return;
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortWidth(S, 1, 1);
    ssSetOutputPortWidth(S, 2, 1);
    ssSetOutputPortWidth(S, 3, 1);
    ssSetOutputPortWidth(S, 4, 1);
    ssSetOutputPortWidth(S, 5, 1);
    ssSetOutputPortWidth(S, 6, 1);
    ssSetOutputPortWidth(S, 7, 1);
    ssSetOutputPortWidth(S, 8, 1);
    ssSetOutputPortWidth(S, 9, 1);
    ssSetOutputPortWidth(S, 10, 1);
    ssSetOutputPortWidth(S, 11, 1);
    ssSetOutputPortWidth(S, 12, 1);
    
    
    
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    
    /* Specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);
    
    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    
}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
/* Function: mdlInitializeConditions ========================================
 * Abstract:
 *    In this function, you should initialize the continuous and discrete
 *    states for your S-function block.  The initial states are placed
 *    in the state vector, ssGetContStates(S) or ssGetRealDiscStates(S).
 *    You can also perform any other initialization activities that your
 *    S-function may require. Note, this routine will be called at the
 *    start of simulation and if it is present in an enabled subsystem
 *    configured to reset states, it will be call when the enabled subsystem
 *    restarts execution to reset the states.
 */
static void mdlInitializeConditions(SimStruct *S)
{
    sw_A_add = 0;
    A1_temp = 0;
    A2_temp = 0;
    A3_temp = 0;
    A4_temp = 0;
    t = 0;
fsin = 0;
PI=3.1415926535;
timeticker = 0;
    
}
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START)
/* Function: mdlStart =======================================================
 * Abstract:
 *    This function is called once at start of model execution. If you
 *    have states that should be initialized once, this is the place
 *    to do it.
 */
static void mdlStart(SimStruct *S)
{
}
#endif /*  MDL_START */



/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    const real_T *u = (const real_T*) ssGetInputPortSignal(S,0);
    real_T       *y0 = ssGetOutputPortSignal(S,0);
    real_T       *y1 = ssGetOutputPortSignal(S,1);
    real_T       *y2 = ssGetOutputPortSignal(S,2);
    real_T       *y3 = ssGetOutputPortSignal(S,3);
    real_T       *y4 = ssGetOutputPortSignal(S,4);
    real_T       *y5 = ssGetOutputPortSignal(S,5);
    real_T       *y6 = ssGetOutputPortSignal(S,6);
    real_T       *y7 = ssGetOutputPortSignal(S,7);
    real_T       *y8 = ssGetOutputPortSignal(S,8);
    real_T       *y9 = ssGetOutputPortSignal(S,9);
    real_T       *y10 = ssGetOutputPortSignal(S,10);
    real_T       *y11 = ssGetOutputPortSignal(S,11);
    real_T       *y12 = ssGetOutputPortSignal(S,12);
    
    RG = u[0];
    
    fsin = 243*sin(RG);
    
    state[0][0][0] = 0;
    state[1][1][0] = 1;
    state[0][1][0] = 2;
    state[0][1][1] = 3;
    state[0][0][1] = 4;
    state[1][0][1] = 5;
    state[1][0][0] = 6;
    state[1][1][1] = 7;
    
    if ( (0 <= RG) && (RG < 0.5101  ) )
    {
        PA = 0;
    }
    else if ( (0.5101  <= RG) && ( RG <0.6849 ) )
        
    {
        PA = 1;
    }
    else if ( (0.6849  <= RG) && ( RG <0.9165 ) )
    {
        PA = 0;
    }
    else if ( (0.9165  <= RG) && ( RG <PI/2 ) )
    {
        PA = 1;
    }
    
//////////////////////////////////////////////////////////////////////////////////////////
    else if ( ((PI/2) <= RG) && ( RG < 2.225092654   ) )
    {
        PA = 1;
    }
    else if ( ( 2.225092654 <= RG) && ( RG < 2.456692654   ) )
    {
        PA = 0;
    }
    else if ( (2.456692654 <= RG) && ( RG < 2.631492654   ) )
    {
        PA = 1;
    }
    else if ( (2.631492654 <= RG) && ( RG < PI   ) )
    {
        PA = 0;
    }
    
////////////////////////////////////////////////////////////////////////
    else if ( (PI <= RG) && ( RG < 3.651692654   ) )
    {
        PA = 0;
    }
    else if ( ( 3.651692654 <= RG) && ( RG < 3.826492654   ) )
    {
        PA = -1;
    }
    else if ( ( RG >= 3.826492654 ) && ( RG < 4.058092654   ) )
    {
        PA = 0;
    }
    else if ( ( 4.058092654 <= RG) && ( RG < 3*PI/2   ) )
    {
        PA = -1;
    }
    
//////////////////////////////////////////////////////////////////////////////////////////
    else if ( (( 3*PI/2 ) <= RG) && ( RG < 5.366685307    ) )
    {
        PA = -1;
    }
    else if ( ( 5.366685307 <= RG) && ( RG < 5.598285307    ) )
    {
        PA = 0;
    }
    else if ( ( 5.598285307 <= RG) && ( RG < 5.773085307    ) )
    {
        PA = -1;
    }
    else if ( ( 5.773085307 <= RG) && ( RG < 2*PI    ) )
    {
        PA = 0;
    }
    
    
    ///////BBBBBBBBBBB/////////////
    
    if ( (0 <= RG) && ( RG <  PI/6   ) )
    {
        PB = -1;
    }
    ////////////////
    else if ( ( PI/6   <= RG) && ( RG < 1.177895102   ) )
    {
        PB = -1;
    }
    else if ( (1.177895102   <= RG) && ( RG < 1.409495102 ) )
    {
        PB = 0;
    }
    else if ( ( 1.40949510  <= RG) && ( RG < 1.584295102   ) )
    {
        PB = -1;
    }
    else if ( (1.584295102   <= RG) && ( RG < (2*PI/3)   ) )
    {
        PB = 0;
    }
    
    ////////////
    else if ( ( (2*PI/3)   <= RG) && ( RG < 2.604495102   ) )
    {
        PB = 0;
    }
    else if ( ( 2.604495102   <= RG) && ( RG < 2.779295102   ) )
    {
        PB = 1;
    }
    else if ( ( 2.779295102   <= RG) && ( RG < 3.010895102   ) )
    {
        PB = 0;
    }
    else if ( ( 3.010895102   <= RG) && ( RG <  (7*PI/6)   ) )
    {
        PB = 1;
    }
    
    //////////////
    else if ( (  (7*PI/6)   <= RG) && ( RG <  4.319487756  ) )
    {
        PB = 1;
    }
    else if ( ( 4.319487756   <= RG) && ( RG <  4.551087756  ) )
    {
        PB = 0;
    }
    else if ( ( 4.551087756   <= RG) && ( RG <  4.725887756  ) )
    {
        PB = 1;
    }
    else if ( ( 4.725887756   <= RG) && ( RG <  (5*PI/3)   ) )
    {
        PB = 0;
    }
    
    //////////////////
    else if ( (  (5*PI/3)    <= RG) && ( RG <  5.746087756   ) )
    {
        PB = 0;
    }
    else if ( (  5.746087756   <= RG) && ( RG <  5.920887756   ) )
    {
        PB = -1;
    }
    else if ( (  5.920887756   <= RG) && ( RG <  6.152487756   ) )
    {
        PB = 0;
    }
    else if ( (  6.152487756   <= RG) && ( RG < 2*PI    ) )
    {
        PB = -1;
    }
    
    
    ////////////CCCCCCCCC/////////////
    
    if ( (0 <= RG) && ( RG < 0.130697551   ) )
    {
        PC = 1;
    }
    else if ( ( 0.130697551   <= RG) && ( RG < 0.362297551   ) )
    {
        PC = 0;
    }
    else if ( (0.362297551   <= RG) && ( RG < 0.537097551   ) )
    {
        PC = 1;
    }
    else if ( (0.537097551   <= RG) && ( RG < (PI/3)  ) )
    {
        PC = 0;
    }
    
    //////
    else if ( (  (PI/3)   <= RG) && ( RG < 1.557297551  ) )
    {
        PC = 0;
    }
    else if ( (1.557297551   <= RG) && ( RG < 1.732097551 ) )
    {
        PC = -1;
    }
    else if ( ( 1.732097551   <= RG) && ( RG < 1.963697551  ) )
    {
        PC = 0;
    }
    else if ( ( 1.963697551   <= RG) && ( RG < ( 5*PI/6 )  ) )
    {
        PC = -1;
    }
   
    ////////////
    else if ( (  ( 5*PI/6 )   <= RG) && ( RG < 3.272290205  ) )
    {
        PC = -1;
    }
    else if ( ( 3.272290205   <= RG) && ( RG < 3.503890205  ) )
    {
        PC = 0;
    }
    else if ( ( 3.503890205   <= RG) && ( RG < 3.678690205  ) )
    {
        PC = -1;
    }
    else if ( ( 3.678690205   <= RG) && ( RG < ( 4*PI/3 )  ) )
    {
        PC = 0;
    }
    
    /////////////
    else if ( (  ( 4*PI/3 )   <= RG) && ( RG < 4.698890205  ) )
    {
        PC = 0;
    }
    else if ( ( 4.698890205   <= RG) && ( RG < 4.873690205  ) )
    {
        PC = 1;
    }
    else if ( ( 4.873690205   <= RG) && ( RG < 5.105290205  ) )
    {
        PC = 0;
    }
    else if ( ( 5.105290205   <= RG) && ( RG <( 11*PI/6 )  ) )
    {
        PC = 1;
    }
    
    /////////
    else if ( ( ( 11*PI/6 )   <= RG) && ( RG <( 2*PI )  ) )
    {
        PC = 1;
    }
    
    
    
    if (PA == 1)
    {
        A1 = 1;
        A2 = 1;
        A3 = 0;
        A4 = 0;
    }
    if (PA == 0)
    {
        A1 = 0;
        A2 = 1;
        A3 = 1;
        A4 = 0;
    }
     if (PA == -1)
    {
        A1 = 0;
        A2 = 0;
        A3 = 1;
        A4 = 1;
    }
    
    if (PB == 1)
    {
        B1 = 1;
        B2 = 1;
        B3 = 0;
        B4 = 0;
    }
    if (PB == 0)
    {
        B1 = 0;
        B2 = 1;
        B3 = 1;
        B4 = 0;
    }
     if (PB == -1)
    {
        B1 = 0;
        B2 = 0;
        B3 = 1;
        B4 = 1;
    }
    
    if (PC == 1)
    {
        C1 = 1;
        C2 = 1;
        C3 = 0;
        C4 = 0;
    }
    if (PC == 0)
    {
        C1 = 0;
        C2 = 1;
        C3 = 1;
        C4 = 0;
    }
     if (PC == -1)
    {
        C1 = 0;
        C2 = 0;
        C3 = 1;
        C4 = 1;
    }
    
    sw_A_add = sw_A_add + abs(A1-A1_temp)+abs(A2-A2_temp)+abs(A3-A3_temp)+abs(A4-A4_temp);
    A1_temp = A1;
    A2_temp = A2;
    A3_temp = A3;
    A4_temp = A4;

    y0[0] = A1;
    y1[0] = A2;       
    y2[0] = A3;       
    y3[0] = A4;       
    y4[0] = B1;      
    y5[0] = B2;      
    y6[0] = B3;
    y7[0] = B4;       
    y8[0] = C1;       
    y9[0] = C2;      
    y10[0] = C3;      
    y11[0] = C4; 
   

}



#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  /* Function: mdlUpdate ======================================================
   * Abstract:
   *    This function is called once for every major integration time step.
   *    Discrete states are typically updated here, but this function is useful
   *    for performing any tasks that should only take place once per
   *    integration step.
   */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
  }
#endif /* MDL_UPDATE */



#define MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives =================================================
   * Abstract:
   *    In this function, you compute the S-function block's derivatives.
   *    The derivatives are placed in the derivative vector, ssGetdX(S).
   */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
}


/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
