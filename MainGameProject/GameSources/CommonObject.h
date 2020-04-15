/*!
@breif �ėp�I�u�W�F�N�g����
@name�@�쐬�ҁF�����B��
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>�ėp�Œ�I�u�W�F�N�g<breif/>
	///<name>�쐬�ҁF�����B��<name/>
	class FixedObj :public ObjectBase
	{
	public:
		FixedObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
				const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr,Position,Rotation,Scale,TexKey,MeshKey){}

		FixedObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr,pNode){}

		virtual ~FixedObj(){}

		void OnCreate()override;
	};

	///<breif>�ėp�ړ��I�u�W�F�N�g<breif/>
	///<name>�쐬�ҁF�����B��<name/>
	enum class MovingType
	{
		Position,
		Rotation,
		Scaling
	};

	class MoveObj :public ObjectBase
	{
	public:
		MoveObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey,const Vec3 Start,const Vec3 End,const float Speed,const float TotalTime);

		MoveObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~MoveObj();

		//�����ݒ�
		void OnCreate()override;

		//�X�V
		void OnUpdate()override;

	private:
		void PosMove();
		bool LerpMove(Vec3 Start,Vec3 end);

		void RotMove();

		MovingType m_Type;
		float m_speed;
		float m_Movetime;
		float m_CurrntTime;
		Vec3 m_Start;
		Vec3 m_End;
	};
}