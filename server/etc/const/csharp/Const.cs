/////////////////////////////////////////////////////
// do not modify the file, gen by const/gen.bat
/////////////////////////////////////////////////////
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

public class Const {
	//��Ϸ����
	public const int ENTITY_STATE_NONE           = 1;              // 
	public const int ENTITY_STATE_BORN           = 2;              // 
	public const int ENTITY_STATE_IDLE           = 3;              // 
	public const int ENTITY_STATE_FIRE           = 4;              // 
	public const int ENTITY_STATE_DEATH          = 5;              // 
	public const int ENTITY_STATE_DEL            = 6;              // 
	//�ӵ�
	public const int BULLET_STATE_NONE           = 7;              // 
	public const int BULLET_STATE_BORN           = 8;              // 
	public const int BULLET_STATE_FLY            = 9;              // 
	public const int BULLET_STATE_HIT            = 10;             // 
	public const int BULLET_STATE_DEL            = 11;             // 
	//����
	public const int ROOM_STATE_WAIT             = 1;              // 
	public const int ROOM_STATE_ING              = 2;              // 
	public const int ROOM_STATE_END              = 3;              // 
	//�ӵ��켣
	public const int BULLET_PATCH_HORIZONTAL     = 1;              // 
	public const int BULLET_PATCH_PARABOLA       = 2;              // 
	//����
	public const int BULLET_MAX_LIFE_TM          = 20;             // ������ʱ��
	public const int ROOM_WAIT_TIME              = 2;              // ��ʼʱ��
	public const int ROOM_ING_TIME               = 300;            // ����ʱ��
	public const int ROOM_END_TIME               = 5;              // ����ʱ��
}