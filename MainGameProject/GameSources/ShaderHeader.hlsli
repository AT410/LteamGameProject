/*
@breif �v���W�F�N�g��`�̃V�F�[�_�[�t�@�C��
@name �쐬�ҁF�����B��
*/

//�R���X�^���g�o�b�t�@
cbuffer ConstantBuffer : register(b0)
{
    float4x4 WorldMatrix : packoffset(c0);
    float4x4 ViewMatrix : packoffset(c4);
    float4x4 ProjectionMatrix : packoffset(c8);
    
    float4 Diffuse : packoffset(c9);
    float4 Emissive : packoffset(c10);
}

//�V�F�[�_�[����p�o�b�t�@
cbuffer ControlBuffer :register(b1)
{
    
}