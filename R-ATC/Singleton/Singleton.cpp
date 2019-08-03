#include "Singleton.h"

class example : public Singleton<example>
{
public:
	friend class Singleton<example>; // Singleton �ł̃C���X�^���X�쐬�͋���

public:
	int public_member;

protected:
	example(); // �O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~example();
};

example::example()
{
}

example::~example()
{
}
