-----------------------------------------------------
-- do not modify the file, gen by const/gen.bat
-----------------------------------------------------
--��Ϸ����
ENTITY_STATE = {}
ENTITY_STATE.NONE           = 1               -- 
ENTITY_STATE.BORN           = 2               -- 
ENTITY_STATE.IDLE           = 3               -- 
ENTITY_STATE.FIRE           = 4               -- 
ENTITY_STATE.DEATH          = 5               -- 
ENTITY_STATE.DEL            = 6               -- 
--�ӵ�
BULLET_STATE = {}
BULLET_STATE.NONE           = 7               -- 
BULLET_STATE.BORN           = 8               -- 
BULLET_STATE.FLY            = 9               -- 
BULLET_STATE.HIT            = 10              -- 
BULLET_STATE.DEL            = 11              -- 
--����
ROOM_STATE = {}
ROOM_STATE.WAIT             = 1               -- 
ROOM_STATE.ING              = 2               -- 
ROOM_STATE.END              = 3               -- 
--�ӵ��켣
BULLET_PATCH = {}
BULLET_PATCH.HORIZONTAL     = 1               -- 
BULLET_PATCH.PARABOLA       = 2               -- 
--����
BULLET_MAX_LIFE_TM          = 20              -- ������ʱ��
ROOM_WAIT_TIME              = 2               -- ��ʼʱ��
ROOM_ING_TIME               = 300             -- ����ʱ��
ROOM_END_TIME               = 5               -- ����ʱ��
