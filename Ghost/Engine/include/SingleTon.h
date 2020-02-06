#ifndef _SINGLE_TON_H_
#define _SINGLE_TON_H_

namespace ghost
{
	template<typename T>
	class SingleTon
	{
	public:
        SingleTon() = default;

		static T* getInstance()
		{
			if (_instance == nullptr)
			{
				_instance = new T();
			}

			return _instance;
		}

		static void destoryInstance()
		{
			if (_instance != nullptr)
			{
				delete _instance;
				_instance = nullptr;
			}
		}

		SingleTon(const T& rhs) = delete;
		SingleTon& operator = (const T& rhs) = delete;

	protected:
		static T* _instance;
	};

	template<typename T>
	T* SingleTon<T>::_instance = nullptr;
}

#endif
