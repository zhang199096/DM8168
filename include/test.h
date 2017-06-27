#ifndef DSP6713_TEST_H
#define DSP6713_TEST_H

extern void test_all(void);
extern void InitTest(void)	;	//for test
extern Uint16 test_PWM(void);
extern Uint16 test_PWM_IO(void);
extern Uint16 test_PWM_CS(void);
extern Uint16 test_X_PWM(void);
extern Uint16 test_Y_PWM(void);
extern Uint16 test_Z_PWM(void);
extern Uint16 test_A_PWM(void);
extern void test_int40(void);
extern void test_servo_io(void);
extern Uint16 test_M2_ini(void);
extern Uint16 test_M2(void);
extern Uint16 test_M2_interpolate(void);
extern Uint16 test_com_ARM(void);
extern Uint16 test_SPC_code(void);
extern Uint16 test_STD_code(void);
extern Uint16	test_int4(void);
extern Uint16 test_IO(void);
extern Uint16 test_VT(void);
extern Uint16 test_sci(void);
extern Int16 GPMC_test(void);
extern Int16 GPMC_test1(void);
extern Int16 GPMC_test2(void);
extern void Test_Dsp_SPC(void);
extern void Test_Dsp_PacketIn(void);

#endif  // end of DSP6713_TEST_H definition
