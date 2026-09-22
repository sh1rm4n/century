//
// Created by sh1rm4n on 6/1/26.
//

#ifndef CENTURY_CGLOBALVARS_HPP
#define CENTURY_CGLOBALVARS_HPP

class CGlobalVars {
public:
    float	realTime;
    int		frameCount;
    float	absoluteFrameTime;
    double	curTime;
    float	frameTime;
    int		maxClients;
    int		tickCount;
    double	intervalPerTick;
    float	interpolationAmount;
    int		simTicksThisFrame;
    int		networkProtocol;
    void*	pSaveData;

private:
    bool	m_bClient;
    int		nTimestampNetworkingBase;
    int		nTimestampRandomizeWindow;
};
#endif //CENTURY_CGLOBALVARS_HPP
