#pragma once
namespace DXLib{
	class DXFPSCounter{
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
