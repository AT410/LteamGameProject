/*!
@file ProjectBehavior.h
@brief �v���W�F�N��`��Component�N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//----------------------------------------------------------------------------
	//�A�N�V�����N���X�̊g��
	//----------------------------------------------------------------------------
	class Actions :public Component
	{
		void RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		bool UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit Actions(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Actions();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�v���邩�ǂ����𓾂�
		@return	���[�v����Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsLooped()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�v���邩�ǂ����𓾂�
		@return	���[�v����Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetLooped()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�v���邩�ǂ�����ݒ肷��
		@param[in]	b	���[�v���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLooped(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����������ǂ����𓾂�<br />
		���[�v�Ȃ��œ���������^�ɂȂ�
		@return	����������true
		*/
		//--------------------------------------------------------------------------------------
		bool IsArrived()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����������ǂ����𓾂�<br />
		���[�v�Ȃ��œ���������^�ɂȂ�
		@return	����������true
		*/
		//--------------------------------------------------------------------------------------
		bool GetArrived()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̑��x�𓾂�<br />
		Move�R���|�[�l���g���Ȃ��ꍇ��bsm::Vec3(0,0,0)��Ԃ�
		@return	���݂̑��x
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetVelocity() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�P�[�����O�A�N�V�����̌��݂̃C���f�b�N�X�𓾂�<br />
		�X�P�[�����O�A�N�V���������݂��Ȃ���Η�O
		@return	���݂̃C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		size_t GetScaleActiveIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��]�A�N�V�����̌��݂̃C���f�b�N�X�𓾂�<br />
		��]�A�N�V���������݂��Ȃ���Η�O
		@return	���݂̃C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		size_t GetRotateActiveIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړ��A�N�V�����̌��݂̃C���f�b�N�X�𓾂�<br />
		�ړ��A�N�V���������݂��Ȃ���Η�O
		@return	���݂̃C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		size_t GetMoveActiveIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ScaleTo�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	TargetScale	�ړI�̃X�P�[��
		@param[in]	Rate = Lerp::Linear	��ԕ��@
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ScaleTo> AddScaleTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetScale, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ScaleBy�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	LocalScale	�ړI�̑��΃X�P�[��
		@param[in]	Rate = Lerp::Linear	��ԕ��@
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ScaleBy> AddScaleBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalScale, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ScaleInterval�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionInterval> AddScaleInterval(wstring ActionKey, float TotalTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肵���C���f�b�N�X�̃X�P�[���R���|�[�l���g�𓾂�
		@param[in]	TargetIndex	�擾����C���f�b�N�X
		@return�@�w�肵���C���f�b�N�X�̃X�P�[���R���|�[�l���g�i�A�N�V�����̃|�C���^�j
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionComponent> GetScaleComponent(size_t TargetIndex) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�ɃL���X�g�����w�肵���C���f�b�N�X�̃X�P�[���R���|�[�l���g�𓾂�
		@tparam	T	�X�P�[���^
		@param[in]	TargetIndex	�擾����C���f�b�N�X
		@return	�w�肵���C���f�b�N�X��T�^�̃R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetScale(size_t TargetIndex) const {
			auto Ptr = GetScaleComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2) {
				throw BaseException(
					L"���̃R���|�[�l���g��T�^�ɃL���X�g�ł��܂���",
					Util::GetWSTypeName<T>(),
					L"Action::GetScale()"
				);
			}
			return Ptr2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�P�[���R���|�[�l���g�̔z��𓾂�<br/>
		���ۂ̔z�񂪕Ԃ�̂ŐT�d�ɑ��삷�邱��
		@return	�X�P�[���R���|�[�l���g�̔z��̎Q��
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<ActionComponent>>& GetScaleVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	RotateTo�A�N�V������ǉ�����<br/>
		�����Ŏ����Ă���̂̓N�I�[�^�j�I���Ȃ̂ŁA�ϊ����ăZ�b�g����
		@param[in]	TotalTime	�����鎞��
		@param[in]	TargetRotate	�ړI�̉�]
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateTo> AddRotateTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetRotate);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	RotateTo�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	Targetbsm::Quat	�ړI�̉�]�N�I�[�^�j�I��
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateTo> AddRotateTo(wstring ActionKey,float TotalTime, const bsm::Quat& TargetQuaternion);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	RotateBy�A�N�V������ǉ�����<br/>
		�����Ŏ����Ă���̂̓N�I�[�^�j�I���Ȃ̂ŁA�ϊ����ăZ�b�g����
		@param[in]	TotalTime	�����鎞��
		@param[in]	LocalRotate	�ړI�̑��Ή�]
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateBy> AddRotateBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalRotate);
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@RotateBy�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	Localbsm::Quat	�ړI�̑��Ή�]�N�I�[�^�j�I��
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateBy> AddRotateBy(wstring ActionKey,float TotalTime, const bsm::Quat& LocalQuaternion);
		//--------------------------------------------------------------------------------------
		/*!
		@brief RotateInterval�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@return�@�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionInterval> AddRotateInterval(wstring ActionKey, float TotalTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w�肵���C���f�b�N�X�̉�]�R���|�[�l���g�𓾂�
		@param[in]	TargetIndex	�擾����C���f�b�N�X
		@return	�w�肵���C���f�b�N�X�̉�]�R���|�[�l���g�i�A�N�V�����̃|�C���^�j
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionComponent> GetRotateComponent(size_t TargetIndex)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�ɃL���X�g�����w�肵���C���f�b�N�X�̉�]�R���|�[�l���g�𓾂�
		@tparam	T	��]�^
		@param[in]	TargetIndex	�擾����C���f�b�N�X
		@return	�w�肵���C���f�b�N�X��T�^�̃R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetRotate(size_t TargetIndex)const {
			auto Ptr = GetRotateComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2) {
				throw BaseException(
					L"���̃R���|�[�l���g��T�^�ɃL���X�g�ł��܂���",
					Util::GetWSTypeName<T>(),
					L"Action::GetRotate()"
				);
			}
			return Ptr2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��]�R���|�[�l���g�̔z��𓾂�<br/>
		���ۂ̔z�񂪕Ԃ�̂ŐT�d�ɑ��삷�邱��
		@return	��]�R���|�[�l���g�̔z��̎Q��
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<ActionComponent>>& GetRotateVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveTo�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	TargetPosition	�ړI�̈ʒu
		@param[in]	Rate = Lerp::Linear	��ԕ��@
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveTo> AddMoveTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveTo�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	TargetPosition	�ړI�̈ʒu
		@param[in]	RateX	X��ԕ��@
		@param[in]	RateY	Y��ԕ��@
		@param[in]	RateZ	Z��ԕ��@
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveTo> AddMoveTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveBy�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	LocalVector	�ړI�̑��Έʒu
		@param[in]	Rate = Lerp::Linear	��ԕ��@
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveBy> AddMoveBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveBy�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@param[in]	LocalVector	�ړI�̑��Έʒu
		@param[in]	RateX	X��ԕ��@
		@param[in]	RateY	Y��ԕ��@
		@param[in]	RateZ	Z��ԕ��@
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveBy> AddMoveBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveInterval�A�N�V������ǉ�����
		@param[in]	TotalTime	�����鎞��
		@return	�ǉ������A�N�V�����̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionInterval> AddMoveInterval(wstring ActionKey,float TotalTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w�肵���C���f�b�N�X�̈ړ��R���|�[�l���g�𓾂�
		@param[in]	TargetIndex	�擾����C���f�b�N�X
		@return	�w�肵���C���f�b�N�X�̈ړ��R���|�[�l���g�i�A�N�V�����̃|�C���^�j
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionComponent> GetMoveComponent(size_t TargetIndex)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief T�^�ɃL���X�g�����w�肵���C���f�b�N�X�̈ړ��R���|�[�l���g�𓾂�
		@tparam	T	�ړ��^
		@param[in]	TargetIndex	�擾����C���f�b�N�X
		@return	�w�肵���C���f�b�N�X��T�^�̃R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetMove(size_t TargetIndex)const {
			auto Ptr = GetMoveComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2) {
				throw BaseException(
					L"���̃R���|�[�l���g��T�^�ɃL���X�g�ł��܂���",
					Util::GetWSTypeName<T>(),
					L"Action::GetMove()"
				);
			}
			return Ptr2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړ��R���|�[�l���g�̔z��𓾂�<br/>
		���ۂ̔z�񂪕Ԃ�̂ŐT�d�ɑ��삷�邱��
		@return	�ړ��R���|�[�l���g�̔z��̎Q��
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<ActionComponent>>& GetMoveVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ׂẴA�N�V�������N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AllActionClear();
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�N�V���������s����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Run(wstring ActionKey, const bool bLoopActive = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�N�V�������~�߂�
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Stop();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�N�V�������ăX�^�[�g����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ReStart();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈���B��֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

	};
}

//end basecross

