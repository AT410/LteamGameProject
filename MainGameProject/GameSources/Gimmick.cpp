/*!
@breif	�M�~�b�N�֘A����
@name	�쐬�ҁF�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//----------------------------------------------------------------------------
	//�X�C�b�`�I�u�W�F�N�g�̎���
	//----------------------------------------------------------------------------
	// -- Xml�}�b�v�f�[�^�p�R���X�g���N�^ --
	SwitchObj::SwitchObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode),m_IsKeep(false)
	{
		m_RecipientKey = XmlDocReader::GetAttribute(pNode, L"EventRecipientKey");
		m_EventMsg = XmlDocReader::GetAttribute(pNode, L"EventMsgStr");
	}

	void SwitchObj::OnCreate()
	{
		DefaultSettings();
		//�`��
		auto drawComp = AddComponent<PNTStaticDraw>();

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();

		//�R���W������t����
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
	}

	void SwitchObj::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//Omori�^�O������������ƏՓ˂���
		if (Other->FindTag(L"EnabledSwitch")) {
			//�X�C�b�`�������ꂽ���̔���
			PostEvent(0.0f, GetThis<SwitchObj>(), m_RecipientKey, m_EventMsg);
			if (!m_Active)
			{
				Vec3 EfkPoint = GetComponent<Transform>()->GetPosition();
				m_Active = true;
				if(!m_ActiveEfk)
					m_ActiveEfk = ObjectFactory::Create<EfkPlay>(L"GOAL_EFK", EfkPoint);
			}
			else
			{
				//m_ActiveEfk.reset();
				//m_Active = false;
			}
			auto PlayerPtr = dynamic_pointer_cast<Player>(Other);
			if (PlayerPtr)
			{
				PlayerPtr->UpdateResetPositon();
			}
		}
	}

	void SwitchObj::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		//Omori�^�O������������ƏՓ˂���
		if (Other->FindTag(L"EnabledSwitch")) {

		}
	}

	void SwitchObj::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		if (Other->FindTag(L"EnabledSwitch")) {
			m_Active = m_IsKeep ? false : m_Active;
			m_ActiveEfk->StopEffect();
		}
	}

	//----------------------------------------------------------------------------
	//���ΐ�����
	//----------------------------------------------------------------------------
	FireLine::FireLine(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode)
	{
		m_RecipientKey = XmlDocReader::GetAttribute(pNode, L"EventRecipientKey");
		m_EventMsg = XmlDocReader::GetAttribute(pNode, L"EventMsgStr");

		auto ConfiguStr = XmlDocReader::GetAttribute(pNode, L"FireLineDirection");
		m_Configu = (FireLineConfigu)_wtoi(ConfiguStr.c_str());
		
	}

	void FireLine::OnCreate()
	{
		//���̏�Ԃ��Ɠ��ΐ��쓮���Ɉʒu�������
		DefaultSettings();
		SetActions();
		//�`�� 
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		//drawComp->SetMeshResource(m_meshKey);
		//drawComp->SetTextureResource(m_texKey);

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();

		//�R���W������t����
		//auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetAfterCollision(AfterCollision::Auto);

	}

	void FireLine::OnUpdate()
	{
		if (m_Active)
		{
			auto ptrTrans = GetComponent<Transform>();
			Vec3 ChangeScale = ptrTrans->GetScale();
			Vec3 MovePos = ptrTrans->GetPosition();

			switch (m_Configu)
			{
			case basecross::FireLine::None:
				break;
			case basecross::FireLine::LeftToRight:
				MovePos.x = FireLineBehaviorPos(-1, MovePos.x);
				ChangeScale.x = FireLineBehaviorScale(ChangeScale.x);
				break;
			case basecross::FireLine::RightToLeft:
				MovePos.x = FireLineBehaviorPos(+1, MovePos.x);
				ChangeScale.x = FireLineBehaviorScale(ChangeScale.x);
				break;
			case basecross::FireLine::UpToDown:
				MovePos.y = FireLineBehaviorPos(-1, MovePos.y);
				ChangeScale.y = FireLineBehaviorScale(ChangeScale.y);
				break;
			case basecross::FireLine::DownToUp:
				MovePos.y = FireLineBehaviorPos(+1, MovePos.y);
				ChangeScale.y = FireLineBehaviorScale(ChangeScale.y);
				break;
			case basecross::FireLine::FrontToBack:
				MovePos.z = FireLineBehaviorPos(+1, MovePos.z);
				ChangeScale.z = FireLineBehaviorScale(ChangeScale.z);
				break;
			case basecross::FireLine::BackToFront:
				MovePos.z = FireLineBehaviorPos(-1, MovePos.z);
				ChangeScale.z = FireLineBehaviorScale(ChangeScale.z);
				break;
			default:
				break;
			}

			if (ChangeScale.x <= 0|| ChangeScale.y <= 0|| ChangeScale.z <= 0)
			{
				m_Active = false;
				PostEvent(0.0f, GetThis<FireLine>(), m_RecipientKey, m_EventMsg);
				GetStage()->RemoveGameObject<FireLine>(GetThis<FireLine>());
			}
			ptrTrans->SetScale(ChangeScale);
			ptrTrans->SetPosition(MovePos);

		}

	}
	
	void FireLine::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"FireSwitch")
		{
			m_Active = true;
		}
	}

	//�M�_
	void HeatStick::OnCreate()
	{
		DefaultSettings();
		SetActions();
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		//drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();

		//�M�_�̃G���A-5�A-1�A�O����|�W�V�����܂�
		//�ω\�ɏC��
		m_HeatArea = AABB(Vec3(-3, -1, 0), m_pos);

		//�R���W������t����
		//auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetFixed(true);
	}


	void HeatStick::OnUpdate()
	{
		if (m_player) {
			//�v���C���[�̃|�W�V�������擾
			auto PlayerPos = m_player->GetComponent<Transform>()->GetPosition();

			//�v���C���[�̒��S����P�C�P�C�P�͈̔�
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			//�v���C���[���M�_�͈̔͂ɓ�������
			if (HitTest::AABB_AABB(m_HeatArea, PlayerAABB)) {
				//���s���Ԍv��
				auto elapsedTime = App::GetApp()->GetElapsedTime();

				m_count += elapsedTime;
				//��T�b��ɏ���
				if (m_count >= 5.0f) {
					if (m_himo) {
						//�Ђ�������
						GetStage()->RemoveGameObject<GameObject>(m_himo->GetThis<GameObject>());
						//�d��̏d�͂��I���ɂ���
					//�d��̏d��
					//������̓o�^
					auto ObjectPtr_O = GetStage()->GetSharedGameObject<ObjectBase>(L"Omori");

					auto OmoriGrav = ObjectPtr_O->GetComponent<Gravity>();
						OmoriGrav->SetUpdateActive(true);
					}
				}
			}
		}
		else
		{
			//�Ђ��̓o�^
			m_himo = GetStage()->GetSharedGameObject<FixedObj>(L"Himo");
			m_player = GetStage()->GetSharedGameObject<Player>(L"Player");
		}
	}


	//�d��
	void Omori::OnCreate()
	{
		DefaultSettings();
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		//�d��
		auto grav = AddComponent<Gravity>();
		//�����ɂ��Ă���
		grav->SetUpdateActive(false);


		//AddTag(L"EnabledSwitch");
		//���L�ݒ�
		//GetStage()->SetSharedGameObject(L"Omori", GetThis<Omori>());
	}

	void Omori::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		GetComponent<Gravity>()->SetGravityZero();
	}


	//������
	void MoveFloor::OnCreate()
	{
		DefaultSettings();
		SetActions();

		if (m_StartActionActive)
		{
			GetComponent<Actions>()->Run(L"Start",m_IsStartActionLoop);
		}
	}
	void MoveFloor::OnUpdate()
	{

	}
	//----------------------------------------------------------------------------
	//�X���[�v�̎���
	//----------------------------------------------------------------------------
	// -- Xml�t�@�C���p�R���X�g���N�^ --
	Slope::Slope(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode)
	{

	}

	void Slope::OnCreate()
	{
		DefaultSettings();
		SetActions();

		auto DrawComp = GetComponent<SmBaseDraw>();
		auto MeshRes = DrawComp->GetMeshResource();
		DrawComp->SetSamplerState(SamplerState::LinearWrap);

		auto Vertex = MeshRes->GetBackupVerteces<VertexPositionNormalTexture>();
		for (int i = 0; i < Vertex.size(); i++)
		{
			auto v = Vertex[i].textureCoordinate;
			float Value = m_scal.length();
			v.x *= Value;
			Vertex[i].textureCoordinate = v;
		}
		//_cnt++;
		DrawComp->UpdateVertices(Vertex);


	}

	void Slope::OnUpdate()
	{
		if (GetComponent<Actions>()->GetArrived())
		{
			GetComponent<Collision>()->SetUpdateActive(true);
		}
	}

	void Slope::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"StartAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"Start",m_IsStartActionLoop);
		}
		else if (event->m_MsgStr == L"EndAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"End");
		}
		else if (event->m_MsgStr == L"TestEvent")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"OnEvent");
		}
	}

	//----------------------------------------------------------------------------
	//���N���X�̎���
	//----------------------------------------------------------------------------
	// -- Xml�t�@�C���p�R���X�g���N�^ --
	Door::Door(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode),m_OpenActive(false),m_MoveEnd(false),m_TotalTime(0.0f)
	{
		
	}

	// -- ������ --
	void Door::OnCreate()
	{
		//// -- �`��ݒ� --
		//auto DrawComp = AddComponent<PNTPointDraw>();
		//DrawComp->SetMeshResource(m_meshKey);
		//DrawComp->SetTextureResource(m_texKey);

		//// -- �z�u�ݒ� --
		//auto transComp = GetComponent<Transform>();
		//transComp->SetPosition(m_pos);
		//transComp->SetScale(m_scal);
		//transComp->SetQuaternion(Quat(m_rot));

		//// -- �������� --
		//auto ColComp = AddComponent<CollisionObb>();
		//ColComp->SetFixed(true);

		//// -- �C�x���g�ݒ� --
		//if (m_EventActive)
		//{
		//	App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(m_ReceiverKey, GetThis<Door>());
		//}

		DefaultSettings();
		SetActions();
	}
	
	// -- �X�V���� --
	void Door::OnUpdate()
	{
		if (m_MoveEnd)
			return;
		auto ActionPtr = GetComponent<Actions>(false);
		if (ActionPtr)
		{
			if(ActionPtr->GetArrived())
				GetComponent<Collision>()->SetUpdateActive(true);
		}

		if (m_OpenActive)
		{
			OpenProcess();
		}
		else
		{
			//CloseProcess();
		}
	}

	void Door::OpenProcess()
	{
		const float AllTime = 5.0f;
		Vec3 Start, End;
		Start = m_pos;
		End = m_pos + Vec3(0, 5, 0);
		m_MoveEnd = MoveBehavior(Start, End, AllTime);
	}

	void Door::CloseProcess()
	{
		const float AllTime = 5.0f;
		Vec3 Start, End;
		Start = m_pos + Vec3(0, 5, 0);
		End = m_pos;
		m_MoveEnd = !MoveBehavior(Start, End, AllTime);
	}

	bool Door::MoveBehavior(Vec3 Start,Vec3 End, const float AllTime)
	{
		float delta = App::GetApp()->GetElapsedTime();
		m_TotalTime += delta;
		if (m_TotalTime > AllTime)
		{
			m_TotalTime = 0.0f;
			return true;
		}

		// -- �C�[�W���O���� --
		Easing<Vec3> easing;
		Vec3 NewPos = easing.EaseInOut(EasingType::Cubic, Start, End, m_TotalTime, AllTime);

		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(NewPos);
		return false;
	}

	// -- �C�x���g�֐� --
	void Door::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"ToActive")
		{
			m_OpenActive = true;
		}
		else if (event->m_MsgStr == L"StartAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"Start");
		}
		else if (event->m_MsgStr == L"EndAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"End");
		}

	}
	//----------------------------------------------------------------------------
	//�����N���X�̎���
	//----------------------------------------------------------------------------
	void Fountain::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetFixed(true);

		//�d��
		auto grav = AddComponent<Gravity>();

		//�����G���A����|�W�V�����܂�
		m_FountainArea = AABB(Vec3(0, -2, 0), 6, 1, 14);
		m_FountainArea2 = AABB(Vec3(-1, -1, -3),Vec3(1, 4, 0));

	}

	void Fountain::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		Vec3 pos = transComp->GetPosition();

		//�v���C���[�̓o�^
		auto ObjectPtr_P = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto stage = GetStage();

		if (ObjectPtr_P) {
			//�v���C���[�̃|�W�V�������擾
			auto PlayerPos = ObjectPtr_P->GetComponent<Transform>()->GetPosition();
			//�v���C���[�̒��S����P�C�P�C�P�͈̔�
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			if (HitTest::AABB_AABB(m_FountainArea, PlayerAABB)) {
				//�v���C���[�̃|�W�V�������X�^�[�g��
				ObjectPtr_P->ResetPositon();

				//�v���C���[������
				//ObjectPtr_P->SetDrawActive(false);
			}
			if (m_Active) {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 3.5f) {
					m_Active = false;
					AddComponent<Gravity>()->StartJump(Vec3(0, 12.5f, 0.0f));
					m_Time = 0;
				}
				else
				{
					if (pos.y < 0)
						return;
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//�v���C���[�̃|�W�V�������X�^�[�g��
						ObjectPtr_P->ResetPositon();

						//�v���C���[������
						//ObjectPtr_P->SetDrawActive(false);
					}
				}
			}
			else {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 1.5f) {
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//�v���C���[�̃|�W�V�������X�^�[�g��
						ObjectPtr_P->ResetPositon();

						//�v���C���[������
						//ObjectPtr_P->SetDrawActive(false);
					}
				}
			}
		}
	}

	void Fountain::OnCollisionEnter(shared_ptr<GameObject>& Other) {

		m_Active = true;
	}


	//-----------------------------------------------------------------------------
	//��̎���
	//-----------------------------------------------------------------------------
	void Waterfall::OnCreate()
	{
		//�x�X�ƏI�_���烁�b�V���̍쐬

		float width = m_Width / 2.0f;

		m_vertices = 
		{
			{Vec3(m_EndPoint.x- width,m_EndPoint.y,0.0f), Vec3(1,0,1),Vec2(0,0)},
			{Vec3(m_EndPoint.x + width,m_EndPoint.y,0.0f), Vec3(1,0,1),Vec2(1,0)},
			{Vec3(m_StartPoint.x - width,m_StartPoint.y,0.0f), Vec3(1,0,1),Vec2(0,1)},
			{Vec3(m_StartPoint.x+ width,m_StartPoint.y,0.0f), Vec3(1,0,1),Vec2(1,1)},
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};


		auto DrawComp = AddComponent<PNTWaterDraw>();
		DrawComp->CreateOriginalMesh<VertexPositionNormalTexture>(m_vertices, indices);
		DrawComp->SetOriginalMeshUse(true);
		DrawComp->SetTextureResource(L"WATER_TX");
		DrawComp->SetSubTexResource(L"WATERSUB_TX");
		DrawComp->SetMaskTexResource(L"WATERMASK_TX");
		DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		DrawComp->SetSamplerState(SamplerState::LinearWrap);

		SetAlphaActive(true);
		
		//�����蔻��

		Vec3 EfkPoint = m_EndPoint;

		//�����Ԃ��̃G�t�F�N�g�̍Đ�
	}



	void Waterfall::OnUpdate()
	{
		auto DrawComp = GetComponent<PNTWaterDraw>();
		m_TotalTime += App::GetApp()->GetElapsedTime()*m_FallSpeed;

		DrawComp->UpdateUV(0.0f, -m_TotalTime);

		DrawComp->UpdateVertices<VertexPositionNormalTexture>(m_vertices);
	}

	WaterJet::WaterJet(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode), m_WaterJetmode(false), m_WaterJetDirToUp(false),
		m_SizeAABBX(2.0f), m_SizeAABBY(20.0f), m_SizeAABBZ(2.0f)
	{

	}
	//��AABBX�p�̃Z�b�^�[�֐�
	//������@����
	//��m_SizeAABBX�̃Z�b�^�[�p�̊֐�
	void WaterJet::SetSizeAABBX(const float sizeX) {
		m_SizeAABBX = sizeX;
	}
	//��AABBY�p�̃Z�b�^�[�֐�
	//������@����
	//��m_SizeAABBY�̃Z�b�^�[�p�̊֐�
	void WaterJet::SetSizeAABBY(const float sizeY) {
		m_SizeAABBY = sizeY;
	}
	//��AABBZ�p�̃Z�b�^�[�֐�
	//������@����
	//��m_SizeAABBZ�̃Z�b�^�[�p�̊֐�
	void WaterJet::SetSizeAABBZ(const float sizeZ) {
		m_SizeAABBZ = sizeZ;
	}

	void WaterJet::OnCreate() {
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(m_meshKey);
		ptrDraw->SetTextureResource(m_texKey);

		auto ptrTransform = AddComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetQuaternion(Quat(m_rot));
		ptrTransform->SetScale(m_scal);

		m_StartPos = ptrTransform->GetPosition();
		if (m_WaterJetDirToUp) {
			m_WaterJetAABB = AABB(Vec3(m_StartPos.x - m_SizeAABBX, m_StartPos.y, m_StartPos.z - m_SizeAABBZ),
				Vec3(m_StartPos.x + m_SizeAABBX, m_StartPos.y + m_SizeAABBY, m_StartPos.z + m_SizeAABBZ));
		}
		else {
			m_WaterJetAABB = AABB(Vec3(m_StartPos.x - m_SizeAABBX, m_StartPos.y - m_SizeAABBY, m_StartPos.z - m_SizeAABBZ),
				Vec3(m_StartPos.x + m_SizeAABBX, m_StartPos.y, m_StartPos.z + m_SizeAABBZ));
		}

		//m_efk = ObjectFactory::Create<EfkPlay>(L"WATERFALL_EFK", m_StartPos);
		
		GetUnderFloor();
		auto Floor = m_Floor.lock();
		Vec3 End = Vec3(m_StartPos.x, m_StartPos.y - m_SizeAABBY / 2, m_StartPos.z);
		if (Floor)
		{
			auto TransComp = GetComponent<Transform>();

			auto FloorAABB = Floor->GetComponent<CollisionObb>()->GetWrappedAABB();
			auto Pos = TransComp->GetPosition();

			Vec3 recVec;
			HitTest::ClosestPtPointAABB(Pos, FloorAABB, recVec);

			End = recVec;
		}

		m_WaterFall = GetStage()->AddGameObject<Waterfall>(Vec3(m_StartPos.x,m_StartPos.y,m_StartPos.z),
			End, 2.0f, 1.0f);
		m_WaterFall->SetDrawActive(m_WaterJetmode);
	}

	void WaterJet::OnUpdate() {
		StartJudgment();
		WaterJetJudgment();
	}

	//���쓮����֐�
	//������@����
	//�������˂̋@�\���쓮�����邩�ǂ����̔��f��������֐�
	void WaterJet::StartJudgment() {
		auto ptrDraw = GetComponent<PNTStaticDraw>();
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		if (m_IntervalFlag)
		{
			m_IntervalTime += Elapsedtime;

			m_FlashTime += Elapsedtime * 5.0f;
			if (m_FlashTime >= XM_2PI) {
				m_FlashTime = 0;
			}
			auto Diffuse = ptrDraw->GetDiffuse();
			Diffuse.y = abs(sin(m_FlashTime));
			ptrDraw->SetDiffuse(Diffuse);

			if (m_IntervalTime > 2.0f)
			{
				m_JudmentTime = 0.0f;
				m_IntervalTime = 0.0f;
				m_IntervalFlag = false;
			}
		}
		else
		{
			m_JudmentTime += Elapsedtime;
			if (m_JudmentTime > 3.0f) {
				m_JudmentTime = 0.0f;
				if (m_WaterJetmode) {
					m_WaterJetmode = false;
					m_WaterFall->SetDrawActive(false);
					m_WaterFall->StopSound();
					ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));
					//m_IntervalFlag = true;
				}
				else {
					m_WaterJetmode = true;
					ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
					m_IntervalFlag = true;
				}
			}
		}
	}

	//�������ːڐG����֐�
	//������@����
	//���쓮���Ă��鐅���˂ƃv���C���[�ɐڐG�����ۂɃv���C���[�ŋN���邱�Ƃ��L�q�����֐�
	void WaterJet::WaterJetJudgment() {
		auto ptrTransform = GetComponent<Transform>();
		m_Pos = ptrTransform->GetPosition();
		auto GetPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = GetPlayer->GetComponent<Transform>()->GetPosition();
		AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
		if (m_WaterJetmode&& !m_IntervalFlag) {
			if (HitTest::AABB_AABB(m_WaterJetAABB, PlayerAABB)) {
				GetPlayer->SetPlayerState(PlayerState::Restart);
			}
			// -- �^���ɃG�t�F�N�g���΂� --
			auto Floor = m_Floor.lock();

			if (Floor)
			{
				auto TransComp = GetComponent<Transform>();

				auto FloorAABB = Floor->GetComponent<CollisionObb>()->GetWrappedAABB();
				auto Pos = TransComp->GetPosition();

				Vec3 recVec;
				HitTest::ClosestPtPointAABB(Pos, FloorAABB, recVec);

				m_WaterFall->UpdateEndPoint(recVec);

				if(!m_efk)
					m_efk = ObjectFactory::Create<EfkPlay>(L"WATERFALL_EFK", recVec);
			}
			else
			{
				GetUnderFloor();
			}

			m_WaterFall->SetDrawActive(true);
			m_WaterFall->StartSound();

		}
		else {
			if (m_efk)
				m_efk.reset();
		}
	}

	void WaterJet::GetUnderFloor()
	{
		Vec3 Near, Far;
		Near = GetComponent<Transform>()->GetPosition();
		Far = Near + Vec3(0, -50, 0);
		for (auto& v : GetStage()->GetGameObjectVec()) {
			if (v->FindTag(L"WaterJetEnd")) {
				auto ColObb = v->GetComponent<CollisionObb>(false);
				if (ColObb) {
					auto Obb = ColObb->GetObb();
					if (HitTest::SEGMENT_OBB(Near, Far, Obb)) {
						m_Floor = v;
					}
				}
			}
		}

	}

	WaterDrop::WaterDrop(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode), m_time(3.0f), m_Cooltime(0.0f), m_CooltimeMax(1.5f)
	{}

	void WaterDrop::OnCreate() {
		AddComponent<Gravity>();
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetQuaternion(Quat(m_rot));
		ptrTransform->SetScale(m_scal);

		auto ptrDraw = AddComponent<PNTPointDraw>();
		ptrDraw->SetMeshResource(m_meshKey);
		ptrDraw->SetTextureResource(m_texKey);

		m_OldPos = ptrTransform->GetPosition();
		m_CurrentPos = ptrTransform->GetPosition();
		AddComponent<CollisionObb>();
	}

	void WaterDrop::OnUpdate() {
		CoolTime();
	}
	//�E�H�[�^�[�h���b�v�N�[���֐�
	//����@����
	//�E�H�[�^�[�h���b�v�̎��ԓI���o�����֐�
	void WaterDrop::CoolTime() {
		auto ptrGrav = GetComponent<Gravity>();
		auto ptrDraw = GetComponent<PNTPointDraw>();
		auto ptrColl = GetComponent<CollisionObb>();
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		if (m_Cooltime > 0.0f) {
			GetComponent<Transform>()->SetPosition(m_pos);

			m_Cooltime += -Elapsedtime;
			ptrGrav->SetUpdateActive(false);
			ptrDraw->SetDrawActive(false);
			ptrColl->SetUpdateActive(false);
		}
		else {
			ptrGrav->SetUpdateActive(true);
			ptrDraw->SetDrawActive(true);
			ptrColl->SetUpdateActive(true);
		}

	}

	void WaterDrop::OnCollisionEnter(shared_ptr<GameObject>&Obj)
	{
		auto PPtr = dynamic_pointer_cast<Player>(Obj);
		if (PPtr)
		{
			PPtr->SetPlayerState(PlayerState::Restart);
		}
		m_Cooltime = m_CooltimeMax;
		App::GetApp()->GetXAudio2Manager()->Start(L"WaterDrop_SD", 0, 0.1f);
	}


	WaterLV::WaterLV(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode)
	{}

	void WaterLV::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetQuaternion(Quat(m_rot));
		ptrTransform->SetScale(m_scal);

		auto ptrScale = ptrTransform->GetScale();
		m_WaterOldPos = ptrTransform->GetPosition();
		m_WaterCurrentPos = ptrTransform->GetPosition();
		m_AABBX = ptrScale.x;
		m_AABBY = ptrScale.y;
		m_AABBZ = ptrScale.z;
		m_PlayerAABBX = ptrScale.x / 2;
		m_PlayerAABBY = ptrScale.y / 2;
		m_PlayerAABBZ = ptrScale.z / 2;
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(m_meshKey);
		ptrDraw->SetTextureResource(m_texKey);

		//m_WaterLVMode = true;

		if (m_EventActive)
		{
			App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(m_ReceiverKey, GetThis<WaterLV>());
		}
	}

	void WaterLV::OnUpdate() {
		WaterLVChange();
		WaterLVJudgement();
	}

	//���E�H�[�^�[���x�����[�h�Z�b�g�֐�
	//������@����
	//bool�^�̃E�H�[�^�[���x�����[�h�̃Z�b�g�֐�
	void WaterLV::SetWaterLVMode(const bool LVMode) {
		m_WaterLVMode = LVMode;
	}
	//���E�H�[�^�[���x���ύX�֐�
	//������@����
	//���E�H�[�^�[���x���̐��ʏ�ԕω����s���֐�
	void WaterLV::WaterLVChange() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto ptrTransform = GetComponent<Transform>();
		auto ptrObjPos = ptrTransform->GetPosition();
		if (m_WaterLVMode) {
			if (m_WaterCurrentPos.y > m_WaterOldPos.y - 4.0f) {
				m_WaterCurrentPos.y += -0.5f * Elapsedtime;
			}
			else if (m_WaterCurrentPos.y < m_WaterOldPos.y - 4.0f) {
				m_WaterTime += Elapsedtime;
			}
			if (m_WaterTime > 6.0f) {
				m_WaterCurrentPos.y += 1.0f * Elapsedtime;
			}
			if (m_WaterCurrentPos.y > m_WaterOldPos.y) {
				m_WaterTime = 0.0f;
				m_WaterCurrentPos.y = m_WaterOldPos.y;
				m_WaterLVMode = false;
			}
		}
		ptrTransform->SetPosition(m_WaterCurrentPos);
	}
	//���E�H�[�^�[���x������֐�
	//������@����
	//���E�H�[�^�[���x���ƃv���C���[�̃{�����[�����E�ɂ�����ڐG������s���֐�
	void WaterLV::WaterLVJudgement() {
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		m_WaterLVAABB = AABB(ptrPos, m_AABBX, m_AABBY, m_AABBZ);
		auto PlayerAABB = AABB(Vec3(PlayerPos.x - m_PlayerAABBX, PlayerPos.y, PlayerPos.z - m_PlayerAABBZ),
			Vec3(PlayerPos.x + m_PlayerAABBX, PlayerPos.y + m_PlayerAABBY, PlayerPos.z + m_PlayerAABBZ));
		if (HitTest::AABB_AABB(m_WaterLVAABB, PlayerAABB)) {
			ptrPlayer->SetPlayerState(PlayerState::Restart);
		}
	}

	void WaterLV::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"WaterDown")
		{
			m_WaterLVMode = true;
		}
	}

	UpDownBox::UpDownBox(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode), m_Speed(2.0f), m_OldPos(0.0f), m_parenttime(0.0f)
	{}

	void UpDownBox::OnCreate() {

		DefaultSettings();
		auto ptrTransform = GetComponent<Transform>();

		//auto ptrPos = ptrTransform->GetPosition();
		//auto ptrColl = AddComponent<CollisionObb>();
		////�����b�V���E�e�N�X�`�������ƃG���[�N�����ăQ�[���ł��Ȃ�
		//auto ptrDraw = AddComponent<PNTPointDraw>();
		//ptrDraw->SetMeshResource(m_meshKey);
		//ptrDraw->SetTextureResource(m_texKey);

		m_OldPos = ptrTransform->GetPosition();
		m_CurrentPos = ptrTransform->GetPosition();

	}

	void UpDownBox::OnUpdate() {
		BoxJudgment();
		//��������
		m_CurrentPos = GetComponent<Transform>()->GetPosition();
		//FloatMove();
	}
	//�{�b�N�X����֐�
	//����@����
	//�v���C���[�����ꂽ���Ƀv���C���[�ƃ{�b�N�X�̐e�q�����������邽�߂̊֐�
	void UpDownBox::BoxJudgment() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto ptrTransform = GetComponent<Transform>();
		if (m_ParentJudge) {
			m_CurrentPos.y += -Elapsedtime;
			ptrTransform->SetPosition(m_CurrentPos);
		}
		else {
			m_parenttime += -Elapsedtime;
			if (m_parenttime <= 0.0f) {
				FloatMove();
			}
			else if (m_parenttime > 0.0f) {
				m_CurrentPos.y += -Elapsedtime;
				ptrTransform->SetPosition(m_CurrentPos);

			}
		}
	}
	//�Փ˔���֐��i�Փ˂��Ă���ԁj
	//����@����
	//�v���C���[�ƏՓ˂����Ƃ��{�b�N�X�͒��݃v���C���[�Ɛe�q�ɂȂ�
	void UpDownBox::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		if (Obj->FindTag(L"EnabledSwitch")) {
			m_parenttime = 2.0f;
			m_ParentJudge = true;
		}
	}
	//�Փ˔���֐��i�Փ˂��痣�ꂽ���j
	//����@����
	//�v���C���[�Ɨ��ꂽ���e�q���������邽�߂�bool�^�̏���������
	void UpDownBox::OnCollisionExit(shared_ptr<GameObject>& Obj) {
		m_ParentJudge = false;
	}

	//���������֐�
	//
	//���������オ��֐�
	bool UpDownBox::FloatMove() {
		//BoxJudgment();
		//��������
		m_totaltime += App::GetApp()->GetElapsedTime();
		if (m_totaltime > 5.0f)
		{
			m_totaltime = 0;
			return true;
		}

		Easing<Vec3> easing;

		auto ep = easing.Linear(m_OldPos, m_CurrentPos, m_totaltime, 5.0);

		GetComponent<Transform>()->SetPosition(ep);
		return false;
	}

	PushObj::PushObj(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode):
	ObjectBase(StagePtr, pNode),m_Boxmode(true)
	{}

	void PushObj::OnCreate() {
		DefaultSettings();
		AddComponent<Gravity>();
	}

	void PushObj::OnUpdate() {
		BoxState();
	}

	//�{�b�N�X��Ԋ֐�
	//����@����
	//�{�b�N�X�̓��������ԂƐÎ~���Ă����Ԃ𕪂��Ă���
	void PushObj::BoxState() {
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		
		if (m_Boxmode) {
			m_CurrentPos = ptrPos;
			m_PastPos = ptrPos;
			ptrTransform->SetPosition(m_CurrentPos);
		}
		else {
			
			ptrTransform->SetPosition(m_StopPos);
		}

	}

	void PushObj::OnCollisionEnter(shared_ptr<GameObject>& Obj) {
		auto ptrPlayer = dynamic_pointer_cast<Player>(Obj);
		auto ptrTransform = GetComponent<Transform>();
		auto ptrFloor = dynamic_pointer_cast<StageTest>(Obj);
		auto ptrPos = ptrTransform->GetPosition();
		if (!ptrPlayer) {
			m_StopPos = m_PastPos;
			m_CurrentPos = m_PastPos;
		}
		else {
			m_StopPos = m_PastPos;
			m_CurrentPos = m_PastPos;
		}

	}

	void PushObj::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		auto ptrPlayer = dynamic_pointer_cast<Player>(Obj);
		auto ptrTransform = GetComponent<Transform>();
			if (ptrPlayer) {
				m_Boxmode = false;
				if (ptrPlayer->GetPushBoxActiv()) {
					m_Boxmode = true;
				}
				else if(!ptrPlayer->GetPushBoxActiv()){
					m_Boxmode = false;
				}
			}
	}

}