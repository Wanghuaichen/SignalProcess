#pragma once


#define KMERROR_SUCCESS				0

#define KMERROR_FILEPATH_TOOSHORT	1	//�ַ��������Ȳ��������ļ�ID
#define KMERROR_FILEID_TOOSHORT		2	//�ַ��������Ȳ��������ļ�ID
#define KMERROR_FILEID_NOTEXIST		7	//���ݿ����ļ�ID������
#define KMERROR_SPACE_NOTENOUGH		8	//���̿ռ䲻��
#define KMERROR_FILE_CANNOTIMPORT	9	//���ļ�û�����
#define KMERROR_FILE_SEARCH_NOTEXIST 11 //��ѯ����Ŀ������

#define KMERROR_USER_CANCEL			14	//�û�ȡ��
#define KMERROR_XMLITEMP_NOTEXIST	17	//��XMLԪ�ص����Բ�����
#define KMERROR_NO_VALUE			18	//ֵΪ��
#define KMERROR_MULTI_VALUE			19	//�޷����ɶ��ֵ
#define KMERROR_OBJECT_NOTFOUND		20	//Ŀ��û�з���,������
#define KMERROR_STATUS_ERROR		21	//״̬����
#define KMERROR_CANNOT_OPERATE		22	//�޷��������,��Ҫչ��һ���ļ������ӣ������ļ�û�������ļ�
#define KMERROR_ARRAY_OVERRIDE		23	//����Խ��

#define KMERROR_FILE_NOTEXIST			4	//�ļ�������
#define KMERROR_FILE_CANNOTMOVE			5	//�޷��ƶ��ļ��������ļ�����ʹ�ã�
#define KMERROR_FILE_ALREADYEXIST		10	//���ļ����Ѿ�����
#define KMERROR_FILE_CANNOTCOPY			12	//�޷������ļ�
#define KMERROR_FILE_ALREADYINBASE		6	//�ĵ��ظ���⣨�ĵ��Ѿ��ڿ��У�
#define KMERROR_FILE_NAMEALREADYINBASE	34	//�ĵ����ظ���⣨�ĵ����Ѿ��ڿ��У�
#define KMERROR_FILE_NAMEALREADYIMPORT	35	//���ĵ����Ѿ����������һ�ο�
#define KMERROR_FILE_NAMEERROR			31	//�ļ�������ȷ
#define KMERROR_FILE_DIRERROR			33	//Ŀ¼����
#define KMERROR_FILE_NEWVERSION			36	//�°汾
#define KMERROR_FILE_INSYSTEMFOLDER		37	//�ļ���ϵͳĿ¼�ڣ�����޷���� 
#define KMERROR_FILE_ISSYSTEMFOLDER		38	//�ļ�ΪϵͳĿ¼��������޷���� 
#define KMERROR_FILE_ISROOTFOLDER		39	//�ļ�Ϊ��Ŀ¼������޷���� 
#define KMERROR_FILE_HASRELATIONSHIP	1100//�ļ�����һЩ�����ĵ�����˲��ܽ���ɾ���Ȳ���
#define KMERROR_FILE_OPERATEFAILURE		1101//�ļ�����������Ϊ���ơ��ƶ����������ȣ�ʧ��
#define KMERROR_FILE_INVALIDNAME		1102//��Ч���ļ���

#define KMERROR_NET_NOTSUPPORT			1201//��֧������


#define KMERROR_XML_FORMATERROR		3	//����XML����ʽ����
#define KMERROR_XMLITEM_NOTEXIST		16	//��XMLԪ�ز�����
#define KMERROR_XMLITEM_ALREADYEXIST	40	//��XMLԪ���Ѿ�����
#define KMERROR_XMLITEM_CHANGED			41	//��XMLԪ���Ѿ�����
#define KMERROR_XMLITEM_NOTCHANGED		42	//��XMLԪ���Ѿ�����



//��ȫ����
#define KMERROR_USER_NOTLOGIN		100	//�û�û�е�¼
#define KMERROR_USER_NOTEXIST		103	//�û�������
#define KMERROR_USER_PASSWDNOTMATCH	104	//�û����벻ƥ��
#define KMERROR_USER_ACCESSDECLINE	105	//Ȩ�޲���
#define KMERROR_USER_REMOTELOGIN	106	//���Ǳ����û���¼


#define KMERROR_CANNOT_CREATE_DIR	101	//���ܽ�����Ŀ¼
#define KMERROR_DIRNAME_TOOLONG		102	//Ŀ¼��̫��

#define KMERROR_GMEM_CREATEFAILURE	120	//����ȫ���ڴ�ʧ��
#define KMERROR_GMEM_LOCKFAILURE	121	//����ȫ���ڴ�ʧ��

#define KMERROR_DB_NOTOPEN				30	//���ݿ�û�д�
#define KMERROR_DB_CANNOTUPDATE			32	//�޷��������ݿ�
#define KMERROR_DB_ALREADYLOGIN			130	//�ĵ����Ѿ���¼
#define KMERROR_DB_ALREADYLOGOUT		141	//�ĵ����Ѿ���¼
#define KMERROR_DB_OTHERUSERLOGIN		136	//�ĵ����Ѿ���¼
#define KMERROR_DB_NOTLOGIN				131	//�ĵ���û�е�¼
#define KMERROR_DB_DATALOGINFAILURE		132	//�����ĵ����¼ʧ��
#define KMERROR_DB_CONFIGLOGINFAILURE	133	//�����ĵ����¼ʧ��
#define KMERROR_DB_WRONGUSER			134	//��ǰ�û�û��Ȩ��
#define KMERROR_DB_NOTFOUND				135	//�ĵ���û�ҵ�
#define KMERROR_DB_ACCESSDECLINE		137	//�ĵ���ܾ�����
#define KMERROR_DB_NAMEALREADYEXIST		138	//�ĵ��������Ѿ�����
#define KMERROR_DB_PATHALREADYEXIST		139	//�ĵ���·���Ѿ��������ĵ���ʹ��
#define KMERROR_DB_FILEIDNOTFOUND		140	//�ĵ�����ĳһ���ĵ�ID�Ҳ���
#define KMERROR_DB_VERSIONCONFLICT		142	//�汾��ͻ��ָĳһ���Ѹ��£�������ʱ����ʹ�����ϰ汾

#define KMERROR_BUFFER_TOSMALL			150	//����ռ�̫С

#define KMERROR_PASSID_INVALIDATE		160	//PassID��Ч

#define KMERROR_REG_NOTFOUND			170	//û�з���ע�����
#define KMERROR_REG_FAILURE				171	//ע��������ʧ��



#define KMERROR_PARAM_ERROR				250	//�����������
#define KMERROR_EXCEPTION				500	//�޷�������쳣����
#define KMERROR_UNKNOWN					255	//δ֪����


#define KMERROR_ARCHIVE_OVERLIMIT		50	//�鵵�ĵ������涨����
#define KMERROR_ARCHIVE_EXIST			51	//�ð汾�Ѿ����˹鵵�ĵ�

#define KMERROR_NAME_ILLEGAL			61	//�ð汾�Ѿ����˹鵵�ĵ�


#define KMERROR_SYSYTEM_NOTINIT			300	//ϵͳû�г�ʼ��

#define KMERROR_LOCAL_ONLY				180//���ޱ��ص���

//extern PA_ERROR s_ErrorInfo[];