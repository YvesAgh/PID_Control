#pragma once


typedef struct pid_struct//��¼ƫ��Ľṹ��
{
	float p = 0;
	float i = 0;
	float d = 0;
	float err = 0;
	float last_err = 0;
	float err_d = 0;
	float err_i = 0;
	int index_enable = 0;//����ֹ��ܿ��� 0Ϊ�رգ�1Ϊ����
	float index_max = 0;//���������
	float index_min = 0;//��������ޣ��������Ϊ�Ǹ�����
}PID_S;



float PID_control_float(float target, float real, PID_S* PID);		//PID������������������Ŀ��ֵ��ʵ��ֵ��PID�ṹ��
float cascadePID_control_float(float target, float real, PID_S* PID,  float index_max, float index_min, //�⻷����
											 float inner_real, PID_S* inner_PID);//�ڻ�����
void param_init(void);															//������ʼ��
