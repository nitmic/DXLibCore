#pragma once
#include <Singleton.hpp>

namespace DXLib{
	class DXFPSCounter : public Singleton<DXFPSCounter>{
		friend Singleton<DXFPSCounter>;
	public:
		void	UpdateFPS();
		void	SetSampleNum(long iSampleNum);

		void	OnKillFocus();
		void	OnSetFocus();
		double	GetAbsoluteTime();

		double	GetElapse()	const;
		double	GetFPS()	const;
		double	GetTime()	const;

		DXFPSCounter();
		~DXFPSCounter();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;
	};
};
