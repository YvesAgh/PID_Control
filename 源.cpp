#include "stdio.h"
#include "pid.h"
#include "math.h"

const float Fre = 1000.0f;//�����ʣ�/hz��
PID_S pid_test;
PID_S pid_inner_test;
void delay(int t)
{
	while (t--);
}




float PID_control_float(float target, float real, PID_S* PID)
{
	float index=1;
	float control_value = 0.0f; //������
	PID->err = target - real;  //�����
	PID->err_d = Fre*(PID->err - PID->last_err);//���仯�ٶ�
	PID->err_i += (PID->err)/Fre;		//�ۼ�ƫ��
	if(PID->index_enable==1)//��������
	{
		if (fabs(PID->err) > PID->index_max)        //����ֹ���
		{
			index = 0;
		}
		else if (fabs(PID->err) < PID->index_min)
		{
			index = 1;
			PID->err_i += PID->err;    	//�ۼ�ƫ��
		}
		else
		{
			index = (PID->index_max - fabs(PID->err)) / (PID->index_max - (PID->index_min));
			PID->err_i += PID->err;    	//�ۼ�ƫ��
		} 
	}
	else index = 1;
	control_value += PID->p * PID->err;	//p����
	control_value += PID->d * PID->err_d;	//d����
	control_value += index * PID->i * PID->err_i;	//i����

	PID->last_err = PID->err;	//��¼�ϴ�����

	return control_value;
}

float cascadePID_control_float(float target, float real, PID_S* PID,  //�⻷����
							   float inner_real, PID_S* inner_PID)//�ڻ�����
{
	return PID_control_float(PID_control_float(target, real, PID),//Ƕ���⻷�����������
							inner_real, inner_PID);
}

void param_init(void)//
{
	pid_test.p = 16;//16
	pid_test.i = 0;
	pid_test.d = 0;
	pid_test.err = 0;
	pid_test.last_err = 0;
	pid_test.err_d = 0;
	pid_test.err_i = 0;
	pid_test.index_enable = 0;
	pid_test.index_max = 100;
	pid_test.index_min = 0;

	pid_inner_test.p = 32;//32
	pid_inner_test.i = 0;//0
	pid_inner_test.d = 0;//0������1�᲻�ȶ�
	pid_inner_test.err = 0;
	pid_inner_test.last_err = 0;
	pid_inner_test.err_d = 0;
	pid_inner_test.err_i = 0;
	pid_test.index_enable = 1;
	pid_inner_test.index_max = 10;
	pid_inner_test.index_min = 5;//����ȡ��errv���ձ仯������ֵ
	//���μ��ɹ�עָ�꣬1��x����Ŀ��ֵ��Ӧ�ٶȺ��ȶ��ԣ�2��errv����0����Ӧ�ٶȺ��ȶ���
	//�����Ƽ�˳��
	//1�����⻷P˳������һ��ֵ����1ʹ��ϵͳ�ܹ���ǿ�ȶ�
	//2�����⻷P���������������Ӧ�ٶȡ���ָ��һ�ε���Ŀ��λ�ã�
	//3������������ڲ����ȶ����Ӵ��ڻ�Pʹϵͳ�ٶȸ����ܹ����õظ���Ŀ���ٶȣ���ʱ��Ƶ�ʿ��ܸ��󣬵����񶯷��ȸ�С��
	//��p�������񶯷������Լ�����
	//4�����ϵͳ���������������ɣ������귢��ϵͳ��С����Ƶ�񶯣����ڻ�d���������0.99��������1����
	//����������δ�ᵽ�Ĳ��������õ���
}


float PID_run(float _x, float _v)
{
	float a = 0;

	return(a = cascadePID_control_float(80, _x, &pid_test,
		_v, &pid_inner_test));//7��1��)


}
int main()//����
{
	//ģ�����ϵͳ
	float a = 0;
	float x = 0;
	float v = 0;
	param_init();
	while (1)
	{
		static int count = 0;
		a = PID_run(x, v);
		v += a/Fre ;
		x += v/Fre ;
		if (count%10 == 0)
		printf("x=%03.0f errv=%03.0f\t", x, pid_inner_test.err);//���ڵ���

		if (count++ == 60)
		{
			printf("\n");
			count = 0;
		}
		for (int i = 0; i < 10; i++)
			delay(500000);
	}
}

