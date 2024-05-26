#pragma once

template<class T>
class CRTP
{
public:
	const T& Underlying() const;
	T& Underlying();
};

template<class T, template<class> class... Ts>
struct CRTPSet : public CRTP<Ts<T>>... {};

template<class T>
inline const T& CRTP<T>::Underlying() const
{
	return static_cast<const T&>(*this);
}
template<class T>
inline T& CRTP<T>::Underlying()
{
	return static_cast<T&>(*this);
}
