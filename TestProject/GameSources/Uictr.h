#pragma once
#include "stdafx.h"

namespace basecross {
	class Uictr : public GameObject // Stage�ɒǉ��������I�u�W�F�N�g�́AGameObject�N���X�̌p���łȂ���΂Ȃ�Ȃ�
	{
	public:
		Uictr(const std::shared_ptr<Stage>& stage) // �K��Stage���󂯎��R���X�g���N�^�����Ȃ���΂Ȃ�Ȃ�
			: GameObject(stage) // �K���e�N���X�̃R���X�g���N�^��Stage��n���ď��������Ȃ���΂Ȃ�Ȃ�
		{
		}

		void OnCreate() override; // �I�u�W�F�N�g�ւ̃R���|�[�l���g�̒ǉ���OnCreate�֐����I�[�o�[���C�h���āA���̒��ōs��
	};
}