ʹ�����codec wm8904���ѵ���ͨ�� ¼���������ܣ�����֧�ֲ�����96K


/******************************************************************/

һ��ap_record��¼���޸�

1���ṩ�ӿ�

/*
function����ʼ��wm8904��I2S����
input��   i2s_sn -- i2s GPIO��ѡ��0 �� 1
ouput:    0 or -1
*/
int Codec_init(uint8 i2s_sn)


/*
function������¼������������WM8904������
input��   bitrate_type -- 0~10,����record_bitrate_e����
ouput:    0 or -1
*/
int Codec_set_brat(uint8 bitrate_type)


/*
function������WM8904��adc����
input��   gain_type -- 0~15,���淶Χ
ouput:    0 or -1
*/
int Codec_set_adcgain(uint8 gain_type)//0~15


/*
function���˳�wm8904��I2S����
ouput:    0 or -1
*/
int Codec_exit(void)

2�����м�����
1)����enable_adc�������ò���1ΪI2S����
2)g_record_vars.ai_source  �����ͱ���Ϊ AI_SOURCE_I2S;�������θ��м��ʹ��


/******************************************************************/
����music_engine�������޸�
1���ṩ�ӿ�

/*
function����ʼ��wm8904��I2S����
input��   i2s_sn -- i2s GPIO��ѡ��0 �� 1
ouput:    0 or -1
*/
int Codec_init(uint8 i2s_sn)


/*
function������WM8904��I2S������
input��   sample_rate -- {8,12,16,24,32,48,96};KHZ
ouput:    0 or -1
*/
int Codec_set_sample(uint16 sample_rate)

/*
function������WM8904��dac����
input��   gain_type -- 0~15,���淶Χ
ouput:    0 or -1
*/
int Codec_set_dacgain(uint8 gain_type)//0~15

/*
function���˳�wm8904��I2S����
ouput:    0 or -1
*/
int Codec_exit(void)

2�����м�����
1)����enable_dac�������ò���1ΪI2S����




������
2013-05-15



