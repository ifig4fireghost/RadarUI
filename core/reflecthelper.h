#ifndef REFLECTHELPER_H
#define REFLECTHELPER_H

#include <QByteArray>
#include <QMetaObject>
#include <QHash>

class ObjectFactory
{
public:
	template<typename T>
	static void registerClass()
	{
		constructors().insert(T::staticMetaObject.className(), &get<T>);
	}

	static QObject* createObject(const QByteArray& className)
	{
		Constructor constructor = constructors().value(className);
		if (constructor == NULL)
			return NULL;
		return (*constructor)();
	}

private:
	typedef QObject* (*Constructor)();

	template<typename T>
	static QObject* get()
	{
		return new T();
	}

	static QHash<QByteArray, Constructor>& constructors()
	{
		static QHash<QByteArray, Constructor> instance;
		return instance;
	}
};
#endif // REFLECTHELPER_H
