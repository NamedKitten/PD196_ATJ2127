tab=app;
DISP_EN=app config;
DISP_CH=Ӧ������;

tab=setting;
DISP_EN=app setting;
DISP_CH=ϵͳ����;

tab=usb;
DISP_EN=usb property;
DISP_CH=usb����;

tab=music;
DISP_EN=music property;
DISP_CH=��������;

tab=photo;
DISP_EN=photo property;
DISP_CH=ͼƬ����;

tab=radio;
DISP_EN=radio property;
DISP_CH=����������;

tab=record;
DISP_EN=record property;
DISP_CH=¼������;

key=CFG_SOFT_WATCHDOG_ENABLE;
type=DATA;
operate=SINGLE;
len=512;
RANGE=0,1;
tabname=app;
DISP_EN=soft watchdog enable:0-disable,1-enable;
DISP_CH=���Ź�����:0-�ر�,1-����

key=MAINMENU_AP_ID_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=app;
DISP_EN=desktop ap array:0-music,1-audible,2-video,3-photo,4-text,5-browser,6-voice,7-record,8-radio,9-tools,10-setting,11,12,13reserve
DISP_CH=������ap˳��:0-����,1-audible,2-��Ƶ,3-ͼƬ,4-������,5-�ļ������,6-����,7-¼��,8-������,9-����,10-����,11,12,13Ԥ��

key=CFG_KEY_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=app;
DISP_EN=key array:0-NULL,1,2,8-reserve,3-mode,4-prev,5-play,6-next,7-V-,9-v+,10-vol
DISP_CH=��������:0-NULL,1,2,8-reserve,3-mode,4-prev,5-play,6-next,7-V-,9-v+,10-vol

key=SETTING_AP_ID_LIGHTNESS;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-11;
tabname=setting;
DISP_EN=backlight;
DISP_CH=����ȼ�;

key=SETTING_AP_ID_SCREEN_SAVER_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-4;
tabname=setting;
DISP_EN=screen save mode:0-none��1-digital clock��2-albumart��3-screen off��4-demo mode
DISP_CH=��Ļ����ģʽ:0-�ޣ�1-����ʱ�ӣ�2-���ͼ����3-�ر���Ļ��4-��ʾģʽ

key=SETTING_AP_ID_LIGHT_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-60(20);
tabname=setting;
DISP_EN=backlight time:1 means 0.5s
DISP_CH=����ʱ��:1��ʾ0.5��

key=SETTING_AP_ID_SCREEN_SAVER_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-180(30);
tabname=setting;
DISP_EN=screen save time:1 means 0.5s
DISP_CH=��Ļ����ʱ��:1��ʾ0.5��

key=SETTING_AP_ID_SLEEP_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-99;
tabname=setting;
DISP_EN=sleep time:1 means 1min
DISP_CH=˯��ʱ��:1��ʾ1����

key=SETTING_AP_ID_TIME_FORMAT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=time format:0-12 hours, 1-24hours
DISP_CH=ʱ���ʽ:0��ʾ12Сʱ�ƣ�1��ʾ24Сʱ��

key=SETTING_AP_ID_DATE_FORMAT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname=setting;
DISP_EN=date format:0��DD_MM_YYYY 1��MM_DD_YYYY 2��YYYY_MM_DD
DISP_CH=���ڸ�ʽ:0��DD_MM_YYYY 1��MM_DD_YYYY 2��YYYY_MM_DD

key=SETTING_AP_ID_AUTORUN_SET;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=auto run:0-disable,1-enable
DISP_CH=�Զ�����ѡ��:0-�ر�,1-����

key=SETTING_AP_ID_SUPPORT_CARD;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=support card:0-not support, 1-support
DISP_CH=֧�ֿ�ѡ��:0-��֧��,1-֧��

key=SETTING_AP_ID_MUSIC_REPEAT_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-3;
tabname=setting;
DISP_EN=music repeat mode:0-disbale,1-repeate one,2-repeate all,3-view playing
DISP_CH=ѭ����ʽ:0-�ر�,1-����ѭ��,2-ȫ��ѭ��,3-�������

key=SETTING_AP_ID_MUSIC_SHUFFLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,16;
tabname=setting;
DISP_EN=shuffle:0-disable,16-enable
DISP_CH=�������:0-�ر�,16-����

key=SETTING_AP_ID_EQ_SETTING;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2,3,4,5,6,7;
tabname=setting;
DISP_EN=eq:0-disable 1-rock 2-funk 3-hip hop 4-jazz 5-classical 6-techno 7-custom;
DISP_CH=eq:0-�ر� 1-ң�� 2-�ſ� 3-ϣ�ջ��� 4-��ʿ 5-�ŵ� 6-������ 7-�Զ���

key=SETTING_AP_ID_VOLUME_LIMIT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-31;
tabname=setting;
DISP_EN=volume limited;
DISP_CH=��������;

key=SETTING_AP_ID_VOLUME_CURRENT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-31;
tabname=setting;
DISP_EN=default volume;
DISP_CH=Ĭ������;

key=SETTING_AP_ID_SLIDE_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=2-30;
tabname=setting;
DISP_EN=slide time;
DISP_CH=ÿ�Żõ�Ƭ����ʱ��;

key=SETTING_AP_ID_SLIDE_REPEAT_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,2;
tabname=setting;
DISP_EN=slide mode:0-disbale, 2-enable
DISP_CH=�õ��ظ�����:0-�ر�,2-����

key=SETTING_AP_ID_SLIDE_SHUFFLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=slide suffle enable:0-disbale, 1-enable
DISP_CH=�õ�������ſ���:0-�ر�,1-����


key=SETTING_AP_ID_POWEROFF_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-240;
tabname=setting;
DISP_EN=power save:1 means 0.5s
DISP_CH=ʡ��ػ�ʱ��:1��ʾ0.5��

key=SETTING_AP_ID_PLAYING_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-90;
tabname=setting;
DISP_EN=back to nowplay time:1 means 0.5s
DISP_CH=�������ڲ��ŵ�ʱ��:1��ʾ0.5��

key=PHOTO_AP_ID_THUMBNAIL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=photo;
DISP_EN=support:0-not support, 1-support
DISP_CH=����ͼģ���Ƿ�֧��:0-��1-��

key=PHOTO_AP_ID_REVOLVE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=photo;
DISP_EN=photo auto rotate:0-no, 1-yes;
DISP_CH=ͼƬ�Ƿ��Զ���ת:0-��1-��

key=MUSIC_LRC_SUPPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=music;
DISP_EN=music lrc support:0-no, 1-yes;
DISP_CH=�����Ƿ�֧�ָ����ʾ:0-��1-��


key=MUSIC_ALBUM_ART_SUPPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=music;
DISP_EN=music album art support:0-no, 1-yes;
DISP_CH=�����Ƿ�֧��ר��ͼƬ��ʾ:0-��1-��

key=RADIO_AP_ID_BAND;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname=radio;
DISP_EN=default band:0-normal��1-japan��2-europe
DISP_CH=��̨Ĭ�ϲ���:0-��ͨ���Σ�1-�ձ����Σ�2-ŷ�޲���

key=RADIO_AP_ID_SEEKMODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
DISP_EN=seek mode:0-hardware mode, 1-software mode
DISP_CH=Ĭ����̨ģʽ:0-Ӳ��seekģʽ��1-���seekģʽ

key=FMENGINE_AP_ID_SEEKTH;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-127;
tabname=radio;
DISP_EN=threshold for auto search
DISP_CH=��̨����

key=RECORD_GAIN_INPUT_LINEIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tabname=record;
DISP_EN=line-n gain:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db
DISP_CH=¼��line-in����:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db

key=RECORD_GAIN_ADC_LINEIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-63;
tabname=record;
DISP_EN=line-in gain:0:-70.5db, step:1.5db
DISP_CH=ADC line-in����:0:-70.5db, ÿ������1.5db

key=RECORD_GAIN_INPUT_MIC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tabname=record;
DISP_EN=mic-in gain:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db
DISP_CH=¼��mic-in����:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db

key=RECORD_GAIN_ADC_MIC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-63;
tabname=record;
DISP_EN=ADC mic-in gain:0:-70.5db, step:1.5db
DISP_CH=ADC mic-in����:0:-70.5db, ÿ������1.5db

key=RECORD_TRACK_LEVEL_BREAKSONG;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-128;
tabname=record;
DISP_EN=threshold for auto breaksong:0-avarage noise energy, none zero -0.5db/step
DISP_CH=�Զ�������ѹ�ż�ֵ:0��ʾ���ó�ʼ¼��������ƽ��ֵ��������-0.5dBһ��

key=RECORD_TRACK_LEVEL_AVR;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-128;
tabname=record;
DISP_EN=threshold for avr:0-avarage noise energy, none zero -0.5db/step
DISP_CH=avr��ѹ�ż�ֵ:0��ʾ���ó�ʼ¼��������ƽ��ֵ��������-0.5dBһ��

key=RECORD_TRACK_DELAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-10;
tabname=record;
DISP_EN=avr delay time:uint: s
DISP_CH=avr��ʱʱ��:��λ��s

key=RECORD_INPUT_SOURCE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tabname=record;
DISP_EN=input source:0:mic-in;1:fm-in;2:line-in
DISP_CH=¼����Դ:0:mic-in;1:fm-in;2:line-in

key=RECORD_FORMAT_TYPE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=record;
DISP_EN=record format:0-wav; 1-mp3
DISP_CH=¼����ʽ:0-wav; 1-mp3

key=RECORD_WAV_BITRATE;
type=DATA;
operate=SINGLE;
len=;
RANGE=5-8;
tabname=record;
DISP_EN=wav bit rate:4:256kbps; 5:512kbps; 6:768kbps; 7:1024kbps; 8:1536kbps
DISP_CH=wav������:4:256kbps; 5:512kbps; 6:768kbps; 7:1024kbps; 8:1536kbps

key=RECORD_MP3_BITRATE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-3;
tabname=record;
DISP_EN=mp3 bit rate:0:32kbps; 1:64kbps; 2:128kbps; 3:192kbps;
DISP_CH=mp3������:0:32kbps; 1:64kbps; 2:128kbps; 3:192kbps;

key=RECORD_TRACK_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=record;
DISP_EN=auto breaksong:0-disable; 1-enable
DISP_CH=�Զ�����ģʽ:0-�ر�;1-����

key=RECORD_AVR_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=record;
DISP_EN=avr mode:0-disable; 1-enable
DISP_CH=avrģʽ:0-�ر�;1-����

key=USB_VID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tabname=usb;
DISP_EN=USB VID
DISP_CH=USB VID

key=USB_PID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tabname=usb;
DISP_EN=USB PID
DISP_CH=USB PID

key=INF_USB_VENDOR;
type=STR;
operate=EDIT;
len=8;
RANGE=;
tabname=usb;
DISP_EN=usb vendor
DISP_CH=usb �豸����

key=INF_USB_PRODUCTID;
type=STR;
operate=EDIT;
len=16;
RANGE=;
tabname=usb;
DISP_EN=usb product id
DISP_CH=usb �豸

key=INF_DEVICE_PRODUCER;
type=STR;
operate=EDIT;
len=32;
RANGE=;
tabname=usb;
DISP_EN=usb producer
DISP_CH=usb ������

key=INF_INQUIRY_INFORMATION_FOB;
type=STR;
operate=EDIT;
len=58;
RANGE=;
tabname=usb;
DISP_EN=infomation of Udisk
DISP_CH=Udisk ��Ϣ

key=INF_INQUIRY_INFORMATION_CDROM;
type=STR;
operate=EDIT;
len=58;
RANGE=;
tabname=usb;
DISP_EN=infomation of CDROM disk
DISP_CH=usb CDROM ����Ϣ

key=INF_CARD_DISPLAY;
type=STR;
operate=EDIT;
len=5;
RANGE=;
tabname=usb;
DISP_EN=display card enable
DISP_CH=�Ƿ���ʾ����

key=INF_UDISK_LABEL;
type=STR;
operate=EDIT;
len=11;
RANGE=;
tabname=usb;
DISP_EN=show udisk label
DISP_CH=���̾����
