//
// Created by sh1rm4n on 6/1/26.
//

#ifndef CENTURY_CGLOBALVARS_HPP
#define CENTURY_CGLOBALVARS_HPP

class CGlobalVars {
public:
    float	realtime;
    int		framecount;
    float	absoluteframetime;
    double	curtime;
    float	frametime;
    int		maxClients;
    int		tickcount;
    double	interval_per_tick;
    float	interpolation_amount;
    int		simTicksThisFrame;
    int		network_protocol;
    void*	pSaveData;

private:
    bool	m_bClient;
    int		nTimestampNetworkingBase;
    int		nTimestampRandomizeWindow;
};
#endif //CENTURY_CGLOBALVARS_HPP
