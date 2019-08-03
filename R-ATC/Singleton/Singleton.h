#pragma once

template<class T>
class Singleton
{
public:
	static inline T& GetInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() {} // �O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) {} // ������Z�q�֎~
	Singleton(const Singleton& obj) {} // �R�s�[�R���X�g���N�^�֎~
};