#pragma once

template<class T>
class Singleton{
public:
	static std::shared_ptr<T> & getInstance(){
		if(m_spInstance == nullptr){
			m_spInstance = std::make_shared<T>();
		}
		return m_spInstance;
	}
protected:
	Singleton();
	virtual ~Singleton(){
		m_spInstance = nullptr;
	}
private:
	Singleton(const Singleton& obj);
	Singleton& operator=(const Singleton& obj);
	static std::shared_ptr<T> m_spInstance;
};

template<class T>
std::shared_ptr<T> Singleton<T>::m_spInstance = nullptr;

template<class T>
std::shared_ptr<T> & GetSingleton(){
	return Singleton<T>::getInstance();
}