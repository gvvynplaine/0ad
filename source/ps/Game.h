/**
 * File        : Game.h
 * Project     : engine
 * Description : Contains the CGame Class which is a representation of the game itself.
 *
 **/
#ifndef _ps_Game_H
#define _ps_Game_H

#include "ps/Errors.h"
#include <vector>

class CWorld;
class CSimulation;
class CGameView;
class CSimulation;
class CPlayer;
class CGameAttributes;

/**
 * Default player limit (not counting the Gaia player)
 * This may be overridden by system.cfg ("max_players")
 **/
#define PS_MAX_PLAYERS 8

/**
 * The container that holds the rules, resources and attributes of the game.
 * The CGame object is responsible for creating a game that is defined by
 * a set of attributes provided. The CGame object is also responsible for
 * maintaining the relations between CPlayer and CWorld, CSimulation and CWorld.
 **/
class CGame : boost::noncopyable
{
	/**
	 * pointer to the CWorld object representing the game world.
	 **/
	CWorld *m_World;
	/**
	 * pointer to the CSimulation object operating on the game world.
	 **/
	CSimulation *m_Simulation;
	/**
	 * pointer to the CGameView object representing the view into the game world.
	 **/
	CGameView *m_GameView;
	/**
	 * pointer to the local CPlayer object operating on the game world.
	 **/
	CPlayer *m_pLocalPlayer;
	/**
	 * STL vectors of pointers to all CPlayer objects(including gaia) operating on the game world.
	 **/
	std::vector<CPlayer *> m_Players;
	/**
	 * number of players operating on the game world(not including gaia).
	 **/
	uint m_NumPlayers;
	/**
	 * the game has been initialized and ready for use if true.
	 **/
	bool m_GameStarted;
	/**
	 * total elapsed game time in seconds.
	 **/
	double m_Time; // needs to be double to get enough precision
	/**
	 * scale multiplier for simulation rate.
	 **/
	float m_SimRate;
	/**
	 * enumerated values for game status.
	 **/
	enum EOG
	{
		EOG_NEUTRAL,	/// Game is in progress
		EOG_DRAW,		/// Game is over as a Draw by means of agreement of civilizations
		EOG_SPECIAL_DRAW,	/// Game is over by players dying at the same time...?
		EOG_LOSE,		/// Game is over, local player loses		
		EOG_WIN			/// Game is over, local player wins
	} GameStatus;
	
public:
	CGame();
	~CGame();

	/**
	 * the game is paused and no updates will be performed if true.
	 **/
	bool m_Paused;

	/*
		Initialize all local state and members for playing a game described by
		the attribute class, and start the game.

		Return: 0 on OK - a PSRETURN code otherwise
	*/
	PSRETURN StartGame(CGameAttributes *pGameAttributes);
	PSRETURN ReallyStartGame();

	/*
		Perform all per-frame updates
	*/
	bool Update(double deltaTime, bool doInterpolate = true);

	void UpdateGameStatus();
	void EndGame();

	/**
	 * Get pointer to the local player object.
	 *
	 * @return CPlayer * the value of m_pLocalPlayer.
	 **/
	inline CPlayer *GetLocalPlayer()
	{	return m_pLocalPlayer; }
	/**
	 * Change the pointer to the local player object.
	 *
	 * @param CPlayer * pLocalPlayer pointer to a valid player object.
	 **/
	inline void SetLocalPlayer(CPlayer *pLocalPlayer)
	{	m_pLocalPlayer=pLocalPlayer; }
	
	// PT: No longer inline, because it does too much error checking. When
	// everything stops trying to access players before they're loaded, feel
	// free to put the inline version back.
	CPlayer *GetPlayer(uint idx);

	/**
	 * Get a reference to the m_Players vector.
	 *
	 * @return std::vector<CPlayer*> * reference to m_Players.
	 **/
	inline std::vector<CPlayer*>* GetPlayers()
	{	return( &m_Players ); }

	/**
	 * Get m_NumPlayers.
	 *
	 * @return PS_uint the value of m_NumPlayers.
	 **/
	inline uint GetNumPlayers() const
	{	return m_NumPlayers; }

	/**
	 * Get m_GameStarted.
	 *
	 * @return bool the value of m_GameStarted.
	 **/
	inline bool IsGameStarted() const
	{
		return m_GameStarted;
	}

	/**
	 * Get the pointer to the game world object.
	 *
	 * @return CWorld * the value of m_World.
	 **/
	inline CWorld *GetWorld()
	{	return m_World; }
	/**
	 * Get the pointer to the game view object.
	 *
	 * @return CGameView * the value of m_GameView.
	 **/
	inline CGameView *GetView()
	{	return m_GameView; }
	/**
	 * Get the pointer to the simulation object.
	 *
	 * @return CSimulation * the value of m_Simulation.
	 **/
	inline CSimulation *GetSimulation()
	{	return m_Simulation; }
	
	/**
	 * Get the current game elapsed time.
	 *
	 * @return double value of m_Time.
	 **/
	inline double GetTime() const
	{	return m_Time; }

	/**
	 * Set the simulation scale multiplier.
	 *
	 * @param float simRate value to set m_SimRate to.
	 *						Because m_SimRate is also used to
	 *						scale TimeSinceLastFrame it must be
	 *						clamped to 0.0f.
	 **/
	inline void SetSimRate(float simRate)
	{	 m_SimRate = std::max(simRate, 0.0f); }
	/**
	 * Get the simulation scale multiplier.
	 *
	 * @return float value of m_SimRate.
	 **/
	inline float GetSimRate() const
	{	return m_SimRate;  }

private:
	PSRETURN RegisterInit(CGameAttributes* pAttribs);
};

extern CGame *g_Game;

#endif
