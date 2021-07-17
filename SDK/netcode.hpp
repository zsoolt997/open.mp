#pragma once

#include "types.hpp"
#include "network.hpp"
#include "player.hpp"
#include "Server/Components/Objects/objects.hpp"

/// Helper macro that reads a bit stream value and returns false on fail
#define CHECKED_READ(output, input) \
	{ \
		NetworkBitStreamValue output ## _in input; \
		if (!bs.read(output ## _in)) { \
			return false; \
		} else { \
			output = std::get<decltype(output)>(output ## _in.data); \
		} \
	}

/// Helper macro that reads a bit stream value and returns false on fail
#define CHECKED_READ_TYPE(output, type, input) \
	{ \
		NetworkBitStreamValue output ## _in input; \
		if (!bs.read(output ## _in)) { \
			return false; \
		} else { \
			output = std::get<type>(output ## _in.data); \
		} \
	}

namespace NetCode {
	namespace RPC {
		struct Invalid final : NetworkPacketBase<0> {
			bool read(INetworkBitStream& bs) {
				assert(false);
				return false;
			}

			void write(INetworkBitStream& bs) const {
				assert(false);
			}
		};

		struct PlayerConnect final : NetworkPacketBase<25> {
			uint32_t VersionNumber;
			uint8_t Modded;
			NetworkString Name;
			uint32_t ChallengeResponse;
			NetworkString Key;
			NetworkString VersionString;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(VersionNumber, { NetworkBitStreamValueType::UINT32 });
				CHECKED_READ(Modded, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(Name, { NetworkBitStreamValueType::DYNAMIC_LEN_STR_8 });
				CHECKED_READ(ChallengeResponse, { NetworkBitStreamValueType::UINT32 });
				CHECKED_READ(Key, { NetworkBitStreamValueType::DYNAMIC_LEN_STR_8 });
				CHECKED_READ(VersionString, { NetworkBitStreamValueType::DYNAMIC_LEN_STR_8 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(VersionNumber));
				bs.write(NetworkBitStreamValue::UINT8(Modded));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(Name));
				bs.write(NetworkBitStreamValue::UINT32(ChallengeResponse));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(Key));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(VersionString));
			}
		};

		struct PlayerJoin final : NetworkPacketBase<137> {
			int PlayerID;
			Colour Col;
			bool IsNPC;
			NetworkString Name;

			bool read(INetworkBitStream& bs) {
				bs.read(PlayerID, NetworkBitStreamValueType::UINT16);
				uint32_t rgba;
				bs.read(rgba, NetworkBitStreamValueType::UINT32);
				Col = Colour::FromRGBA(rgba);
				bs.read(IsNPC, NetworkBitStreamValueType::UINT8);
				bs.read(Name, NetworkBitStreamValueType::DYNAMIC_LEN_STR_8);
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(PlayerID)));
				bs.write(NetworkBitStreamValue::UINT32(Col.RGBA()));
				bs.write(NetworkBitStreamValue::UINT8(IsNPC));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(Name));
			}
		};

		struct PlayerQuit final : NetworkPacketBase<138> {
			int PlayerID;
			uint8_t Reason;

			bool read(INetworkBitStream& bs) {
				bs.read(PlayerID, NetworkBitStreamValueType::UINT16);
				bs.read(Reason, NetworkBitStreamValueType::UINT8);
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(PlayerID)));
				bs.write(NetworkBitStreamValue::UINT8(Reason));
			}
		};

		struct PlayerInit final : NetworkPacketBase<139> {
			bool         EnableZoneNames;
			bool         UsePlayerPedAnims;
			bool         AllowInteriorWeapons;
			bool         UseLimitGlobalChatRadius;
			float        LimitGlobalChatRadius;
			bool         EnableStuntBonus;
			float        SetNameTagDrawDistance;
			bool         DisableInteriorEnterExits;
			bool         DisableNameTagLOS;
			bool         ManualVehicleEngineAndLights;
			uint32_t     SetSpawnInfoCount;
			int          PlayerID;
			bool         ShowNameTags;
			uint32_t     ShowPlayerMarkers;
			uint8_t      SetWorldTime;
			uint8_t      SetWeather;
			float        SetGravity;
			bool         LanMode;
			uint32_t     SetDeathDropAmount;
			bool         Instagib;
			uint32_t     OnFootRate;
			uint32_t     InCarRate;
			uint32_t     WeaponRate;
			uint32_t     Multiplier;
			uint32_t     LagCompensation;
			NetworkString  ServerName;
			NetworkArray<uint8_t> VehicleModels;

			bool read(INetworkBitStream& bs) {
				bs.read(EnableZoneNames, NetworkBitStreamValueType::BIT);
				bs.read(UsePlayerPedAnims, NetworkBitStreamValueType::BIT);
				bs.read(AllowInteriorWeapons, NetworkBitStreamValueType::BIT);
				bs.read(UseLimitGlobalChatRadius, NetworkBitStreamValueType::BIT);
				bs.read(LimitGlobalChatRadius, NetworkBitStreamValueType::FLOAT);
				bs.read(EnableStuntBonus, NetworkBitStreamValueType::BIT);
				bs.read(SetNameTagDrawDistance, NetworkBitStreamValueType::FLOAT);
				bs.read(DisableInteriorEnterExits, NetworkBitStreamValueType::BIT);
				bs.read(DisableNameTagLOS, NetworkBitStreamValueType::BIT);
				bs.read(ManualVehicleEngineAndLights, NetworkBitStreamValueType::BIT);
				bs.read(SetSpawnInfoCount, NetworkBitStreamValueType::UINT32);
				bs.read(PlayerID, NetworkBitStreamValueType::UINT16);
				bs.read(ShowNameTags, NetworkBitStreamValueType::BIT);
				bs.read(ShowPlayerMarkers, NetworkBitStreamValueType::UINT32);
				bs.read(SetWorldTime, NetworkBitStreamValueType::UINT8);
				bs.read(SetWeather, NetworkBitStreamValueType::UINT8);
				bs.read(SetGravity, NetworkBitStreamValueType::FLOAT);
				bs.read(LanMode, NetworkBitStreamValueType::BIT);
				bs.read(SetDeathDropAmount, NetworkBitStreamValueType::UINT32);
				bs.read(Instagib, NetworkBitStreamValueType::BIT);
				bs.read(OnFootRate, NetworkBitStreamValueType::UINT32);
				bs.read(InCarRate, NetworkBitStreamValueType::UINT32);
				bs.read(WeaponRate, NetworkBitStreamValueType::UINT32);
				bs.read(Multiplier, NetworkBitStreamValueType::UINT32);
				bs.read(LagCompensation, NetworkBitStreamValueType::UINT32);
				bs.read(ServerName, NetworkBitStreamValueType::DYNAMIC_LEN_STR_8);
				bs.read(VehicleModels, NetworkBitStreamValueType::FIXED_LEN_ARR_UINT8);
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::BIT(EnableZoneNames));
				bs.write(NetworkBitStreamValue::BIT(UsePlayerPedAnims));
				bs.write(NetworkBitStreamValue::BIT(AllowInteriorWeapons));
				bs.write(NetworkBitStreamValue::BIT(UseLimitGlobalChatRadius));
				bs.write(NetworkBitStreamValue::FLOAT(LimitGlobalChatRadius));
				bs.write(NetworkBitStreamValue::BIT(EnableStuntBonus));
				bs.write(NetworkBitStreamValue::FLOAT(SetNameTagDrawDistance));
				bs.write(NetworkBitStreamValue::BIT(DisableInteriorEnterExits));
				bs.write(NetworkBitStreamValue::BIT(DisableNameTagLOS));
				bs.write(NetworkBitStreamValue::BIT(ManualVehicleEngineAndLights));
				bs.write(NetworkBitStreamValue::UINT32(SetSpawnInfoCount));
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(PlayerID)));
				bs.write(NetworkBitStreamValue::BIT(ShowNameTags));
				bs.write(NetworkBitStreamValue::UINT32(ShowPlayerMarkers));
				bs.write(NetworkBitStreamValue::UINT8(SetWorldTime));
				bs.write(NetworkBitStreamValue::UINT8(SetWeather));
				bs.write(NetworkBitStreamValue::FLOAT(SetGravity));
				bs.write(NetworkBitStreamValue::BIT(LanMode));
				bs.write(NetworkBitStreamValue::UINT32(SetDeathDropAmount));
				bs.write(NetworkBitStreamValue::BIT(Instagib));
				bs.write(NetworkBitStreamValue::UINT32(OnFootRate));
				bs.write(NetworkBitStreamValue::UINT32(InCarRate));
				bs.write(NetworkBitStreamValue::UINT32(WeaponRate));
				bs.write(NetworkBitStreamValue::UINT32(Multiplier));
				bs.write(NetworkBitStreamValue::UINT32(LagCompensation));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(ServerName));
				bs.write(NetworkBitStreamValue::FIXED_LEN_ARR_UINT8(VehicleModels));
			}
		};

		struct PlayerRequestClass final : NetworkPacketBase<128> {
			int Classid;

			bool read(INetworkBitStream& bs) {
				uint16_t classid;
				CHECKED_READ(classid, { NetworkBitStreamValueType::UINT16 });
				Classid = classid;
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(Classid));
			}
		};

		struct PlayerRequestClassResponse final : NetworkPacketBase<128> {
			uint8_t Selectable;
			uint8_t TeamID;
			uint32_t ModelID;
			uint8_t Unknown1;
			glm::vec3 Spawn;
			float ZAngle;
			NetworkArray<uint32_t> Weapons;
			NetworkArray<uint32_t> Ammos;

			/// Default constructor
			PlayerRequestClassResponse() {}

			/// Construction from a IClass
			PlayerRequestClassResponse(int team, int model, Vector3 spawn, float angle) {
				TeamID = team;
				ModelID = model;
				Spawn = spawn;
				ZAngle = angle;
			}

			bool read(INetworkBitStream& bs) {
				bs.read(Selectable, NetworkBitStreamValueType::UINT8);
				bs.read(TeamID, NetworkBitStreamValueType::UINT8);
				bs.read(ModelID, NetworkBitStreamValueType::UINT32);
				bs.read(Unknown1, NetworkBitStreamValueType::UINT8);
				bs.read(Spawn, NetworkBitStreamValueType::VEC3);
				bs.read(ZAngle, NetworkBitStreamValueType::FLOAT);
				bs.read(Weapons, NetworkBitStreamValueType::FIXED_LEN_ARR_UINT32);
				bs.read(Ammos, NetworkBitStreamValueType::FIXED_LEN_ARR_UINT32);
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Selectable));
				bs.write(NetworkBitStreamValue::UINT8(TeamID));
				bs.write(NetworkBitStreamValue::UINT32(ModelID));
				bs.write(NetworkBitStreamValue::UINT8(Unknown1));
				bs.write(NetworkBitStreamValue::VEC3(Spawn));
				bs.write(NetworkBitStreamValue::FLOAT(ZAngle));
				bs.write(NetworkBitStreamValue::FIXED_LEN_ARR_UINT32(Weapons));
				bs.write(NetworkBitStreamValue::FIXED_LEN_ARR_UINT32(Ammos));
			}
		};

		struct SetSpawnInfo final : NetworkPacketBase<68> {
			uint8_t TeamID;
			uint32_t ModelID;
			uint8_t Unknown1;
			Vector3 Spawn;
			float ZAngle;
			NetworkArray<uint8_t> Weapons;
			NetworkArray<uint32_t> Ammos;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(TeamID));
				bs.write(NetworkBitStreamValue::UINT32(ModelID));
				bs.write(NetworkBitStreamValue::UINT8(Unknown1));
				bs.write(NetworkBitStreamValue::VEC3(Spawn));
				bs.write(NetworkBitStreamValue::FLOAT(ZAngle));
				bs.write(NetworkBitStreamValue::FIXED_LEN_ARR_UINT8(Weapons));
				bs.write(NetworkBitStreamValue::FIXED_LEN_ARR_UINT32(Ammos));
			}
		};

		struct PlayerRequestSpawn final : NetworkPacketBase<129> {
			bool read(INetworkBitStream& bs) {
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerRequestSpawnResponse final : NetworkPacketBase<129> {
			uint32_t Allow;

			bool read(INetworkBitStream& bs) {
				bs.read(Allow, NetworkBitStreamValueType::UINT32);
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(Allow));
			}
		};



		struct PlayerSpawn final : NetworkPacketBase<52> {
			bool read(INetworkBitStream& bs) {
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct SetCheckpoint final : NetworkPacketBase<107> {
			Vector3 position;
			float size;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC3(position));
				bs.write(NetworkBitStreamValue::FLOAT(size));
			}
		};

		struct DisableCheckpoint final : NetworkPacketBase<37> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {}
		};

		struct SetRaceCheckpoint final : NetworkPacketBase<38> {
			uint8_t type;
			Vector3 position;
			Vector3 nextPosition;
			float size;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(type));
				bs.write(NetworkBitStreamValue::VEC3(position));
				bs.write(NetworkBitStreamValue::VEC3(nextPosition));
				bs.write(NetworkBitStreamValue::FLOAT(size));
			}
		};

		struct DisableRaceCheckpoint final : NetworkPacketBase<39> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {}
		};

		struct GivePlayerWeapon final : NetworkPacketBase<22> {
			uint32_t Weapon;
			uint32_t Ammo;

			bool read(INetworkBitStream& bs) {
				bs.read(Weapon, NetworkBitStreamValueType::UINT32);
				bs.read(Ammo, NetworkBitStreamValueType::UINT32);
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(Weapon));
				bs.write(NetworkBitStreamValue::UINT32(Ammo));
			}
		};

		struct ResetPlayerWeapons final : NetworkPacketBase<21> {
			bool read(INetworkBitStream& bs) {
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct SetPlayerArmedWeapon final : NetworkPacketBase<67> {
			uint32_t Weapon;

			bool read(INetworkBitStream& bs) {
				bs.read(Weapon, NetworkBitStreamValueType::UINT32);
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(Weapon));
			}
		};

		struct PlayerStreamIn final : NetworkPacketBase<32> {
			int PlayerID;
			uint8_t Team;
			uint32_t Skin;
			Vector3 Pos;
			float Angle;
			Colour Col;
			uint8_t FightingStyle;
			NetworkArray<uint16_t> SkillLevel;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT8(Team));
				bs.write(NetworkBitStreamValue::UINT32(Skin));
				bs.write(NetworkBitStreamValue::VEC3(Pos));
				bs.write(NetworkBitStreamValue::FLOAT(Angle));
				bs.write(NetworkBitStreamValue::UINT32(Col.RGBA()));
				bs.write(NetworkBitStreamValue::UINT8(FightingStyle));
				bs.write(NetworkBitStreamValue::FIXED_LEN_ARR_UINT16(SkillLevel));
			}
		};

		struct PlayerStreamOut final : NetworkPacketBase<163> {
			int PlayerID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
			}
		};

		struct SetPlayerName final : NetworkPacketBase<11> {
			int PlayerID;
			NetworkString Name;
			uint8_t Success;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(Name));
				bs.write(NetworkBitStreamValue::UINT8(Success));
			}
		};

		struct SendClientMessage final : NetworkPacketBase<93> {
			NetworkString Message;
			Colour Col;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(Col.RGBA()));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_32(Message));
			}
		};

		struct PlayerRequestChatMessage final : NetworkPacketBase<101> {
			NetworkString message;
			bool read(INetworkBitStream& bs) {
				CHECKED_READ(message, { NetworkBitStreamValueType::DYNAMIC_LEN_STR_8 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerChatMessage final : NetworkPacketBase<101> {
			int PlayerID;
			NetworkString message;
			bool read(INetworkBitStream& bs) {
				return false;
			}
			
			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(message));
			}
		};

		struct PlayerRequestCommandMessage final : NetworkPacketBase<50> {
			NetworkString message;
			bool read(INetworkBitStream& bs) {
				CHECKED_READ(message, { NetworkBitStreamValueType::DYNAMIC_LEN_STR_32 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerCommandMessage final : NetworkPacketBase<50> {
			NetworkString message;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_32(message));
			}
		};

		struct SendDeathMessage final : NetworkPacketBase<55> {
			int KillerID;
			int PlayerID;
			int reason;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(KillerID));
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT8(reason));
			}
		};


		struct SetPlayerWeather final : NetworkPacketBase<152> {
			uint8_t WeatherID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(WeatherID));
			}
		};

		struct SetWorldBounds final : NetworkPacketBase<17> {
			Vector4 coords;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC4(coords));
			}
		};


		struct SetPlayerColor final : NetworkPacketBase<72> {
			int PlayerID;
			Colour Col;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT32(Col.RGBA()));
			}
		};

		struct SetPlayerPosition final : NetworkPacketBase<12> {
			Vector3 Pos;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC3(Pos));
			}
		};

		struct SetPlayerCameraPosition final : NetworkPacketBase<157> {
			Vector3 Pos;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC3(Pos));
			}
		};

		struct SetPlayerCameraLookAt final : NetworkPacketBase<158> {
			Vector3 Pos;
			uint8_t cutType;
			

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC3(Pos));
			}
		};

		struct SetPlayerCameraBehindPlayer final : NetworkPacketBase<162> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct SetPlayerPositionFindZ final : NetworkPacketBase<13> {
			Vector3 Pos;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC3(Pos));
			}
		};

		struct SetPlayerFacingAngle final : NetworkPacketBase<19> {
			float Angle;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::FLOAT(Angle));
			}
		};

		struct SetPlayerTeam final : NetworkPacketBase<69> {
			int PlayerID;
			uint8_t Team;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT8(Team));
			}
		};

		struct SetPlayerFightingStyle final : NetworkPacketBase<89> {
			int PlayerID;
			uint8_t Style;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT8(Style));
			}
		};

		struct SetPlayerSkillLevel final : NetworkPacketBase<34> {
			int PlayerID;
			uint32_t SkillType;
			uint16_t SkillLevel;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT32(SkillType));
				bs.write(NetworkBitStreamValue::UINT16(SkillLevel));
			}
		};

		struct SetPlayerSkin final : NetworkPacketBase<153> {
			int PlayerID;
			uint32_t Skin;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(PlayerID));
				bs.write(NetworkBitStreamValue::UINT32(Skin));
			}
		};

		struct SetPlayerHealth final : NetworkPacketBase<14> {
			float Health;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::FLOAT(Health));
			}
		};

		struct SetPlayerArmour final : NetworkPacketBase<66> {
			float Armour;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::FLOAT(Armour));
			}
		};

		struct SetPlayerSpecialAction final : NetworkPacketBase<88> {
			int Action;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Action));
			}
		};

		struct SetPlayerVelocity final : NetworkPacketBase<90> {
			Vector3 Velocity;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC3(Velocity));
			}
		};

		struct ApplyPlayerAnimation final : NetworkPacketBase<86> {
			int PlayerID;
			NetworkString AnimLib;
			NetworkString AnimName;
			float Delta;
			bool Loop;
			bool LockX;
			bool LockY;
			bool Freeze;
			uint32_t Time;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(AnimLib));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(AnimName));
				bs.write(NetworkBitStreamValue::FLOAT(Delta));
				bs.write(NetworkBitStreamValue::BIT(Loop));
				bs.write(NetworkBitStreamValue::BIT(LockX));
				bs.write(NetworkBitStreamValue::BIT(LockY));
				bs.write(NetworkBitStreamValue::BIT(Freeze));
				bs.write(NetworkBitStreamValue::UINT32(Time));
			}
		};

		struct ClearPlayerAnimations final : NetworkPacketBase<87> {
			int PlayerID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
			}
		};

		struct TogglePlayerControllable final : NetworkPacketBase<15> {
			bool Enable;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Enable));
			}
		};

		struct TogglePlayerSpectating final : NetworkPacketBase<124> {
			bool Enable;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(Enable));
			}
		};

		struct PlayerPlaySound final : NetworkPacketBase<16> {
			uint32_t SoundID;
			Vector3 Position;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(SoundID));
				bs.write(NetworkBitStreamValue::VEC3(Position));
			}
		};

		struct GivePlayerMoney final : NetworkPacketBase<18> {
			int32_t Money;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::INT32(Money));
			}
		};

		struct ResetPlayerMoney final : NetworkPacketBase<20> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct SetPlayerTime final : NetworkPacketBase<29> {
			uint8_t Hour;
			uint8_t Minute;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Hour));
				bs.write(NetworkBitStreamValue::UINT8(Minute));
			}
		};

		struct TogglePlayerClock final : NetworkPacketBase<30> {
			bool Toggle;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Toggle));
			}
		};

		struct OnPlayerDeath final : NetworkPacketBase<53> {
			uint8_t Reason;
			uint16_t KillerID;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(Reason, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(KillerID, { NetworkBitStreamValueType::UINT16 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerDeath final : NetworkPacketBase<166> {
			int PlayerID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
			}
		};

		struct SetPlayerShopName final : NetworkPacketBase<33> {
			NetworkString Name;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				String nameFixed(Name);
				nameFixed.resize(0x20);
				bs.write(NetworkBitStreamValue::FIXED_LEN_STR(NetworkString(nameFixed)));
			}
		};

		struct SetPlayerDrunkLevel final : NetworkPacketBase<35> {
			int32_t Level;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::INT32(Level));
			}
		};

		struct PlayAudioStreamForPlayer final : NetworkPacketBase<41> {
			NetworkString URL;
			Vector3 Position;
			float Distance;
			bool Usepos;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(URL));
				bs.write(NetworkBitStreamValue::VEC3(Position));
				bs.write(NetworkBitStreamValue::FLOAT(Distance));
				bs.write(NetworkBitStreamValue::UINT8(Usepos));
			}
		};

		struct StopAudioStreamForPlayer final : NetworkPacketBase<42> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct SetPlayerAmmo final : NetworkPacketBase<145> {
			uint8_t Weapon;
			uint16_t Ammo;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Weapon));
				bs.write(NetworkBitStreamValue::UINT16(Ammo));
			}
		};

		struct SendPlayerScoresAndPings final : NetworkPacketBase<155> {
			int PlayerID;
			int32_t Score;
			uint32_t Ping;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::INT32(Score));
				bs.write(NetworkBitStreamValue::UINT32(Ping));
			}
		};

		struct OnPlayerRequestScoresAndPings final : NetworkPacketBase<155> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct RemoveBuildingForPlayer final : NetworkPacketBase<43> {
			unsigned ModelID;
			Vector3 Position;
			float Radius;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(ModelID));
				bs.write(NetworkBitStreamValue::VEC3(Position));
				bs.write(NetworkBitStreamValue::FLOAT(Radius));
			}
		};

		struct CreateExplosion final : NetworkPacketBase<79> {
			Vector3 vec;
			uint16_t type;
			float radius;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::VEC3(vec));
				bs.write(NetworkBitStreamValue::UINT16(type));
				bs.write(NetworkBitStreamValue::FLOAT(radius));
			}
		};

		struct ForcePlayerClassSelection final : NetworkPacketBase<74> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct SetPlayerInterior final : NetworkPacketBase<156> {
			unsigned Interior;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Interior));
			}
		};

		struct SetPlayerWantedLevel final : NetworkPacketBase<133> {
			unsigned Level;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(Level));
			}
		};

		struct ToggleWidescreen final : NetworkPacketBase<111> {
			bool enable;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::BIT(enable));
			}
		};

		struct OnPlayerGiveTakeDamage final : NetworkPacketBase<115> {
			bool Taking;
			int PlayerID;
			float Damage;
			uint32_t WeaponID;
			uint32_t Bodypart;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(Taking, { NetworkBitStreamValueType::BIT });
				CHECKED_READ_TYPE(PlayerID, uint16_t, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(Damage, { NetworkBitStreamValueType::FLOAT });
				CHECKED_READ(WeaponID, { NetworkBitStreamValueType::UINT32 });
				CHECKED_READ(Bodypart, { NetworkBitStreamValueType::UINT32 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct OnPlayerInteriorChange final : NetworkPacketBase<118> {
			unsigned Interior;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ_TYPE(Interior, uint8_t, { NetworkBitStreamValueType::UINT8 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct StreamInVehicle final : NetworkPacketBase<164> {
			int VehicleID;
			int ModelID;
			Vector3 Position;
			float Angle;
			uint8_t Colour1;
			uint8_t Colour2;
			float Health;
			uint8_t	Interior;
			uint32_t DoorDamage;
			uint32_t PanelDamage;
			uint8_t LightDamage;
			uint8_t TyreDamage;
			uint8_t Siren;
			std::array<int, 14> Mods;
			uint8_t Paintjob;
			int32_t BodyColour1;
			int32_t BodyColour2;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(VehicleID));
				bs.write(NetworkBitStreamValue::UINT32(ModelID));
				bs.write(NetworkBitStreamValue::VEC3(Position));
				bs.write(NetworkBitStreamValue::FLOAT(Angle));
				bs.write(NetworkBitStreamValue::UINT8(Colour1));
				bs.write(NetworkBitStreamValue::UINT8(Colour2));
				bs.write(NetworkBitStreamValue::FLOAT(Health));
				bs.write(NetworkBitStreamValue::UINT8(Interior));
				bs.write(NetworkBitStreamValue::UINT32(DoorDamage));
				bs.write(NetworkBitStreamValue::UINT32(PanelDamage));
				bs.write(NetworkBitStreamValue::UINT8(LightDamage));
				bs.write(NetworkBitStreamValue::UINT8(TyreDamage));
				bs.write(NetworkBitStreamValue::UINT8(Siren));
				for (int mod : Mods) {
					bs.write(NetworkBitStreamValue::UINT8(mod ? uint8_t(mod - 999) : 0));
				}
				bs.write(NetworkBitStreamValue::UINT8(Paintjob));
				bs.write(NetworkBitStreamValue::UINT32(BodyColour1));
				bs.write(NetworkBitStreamValue::UINT32(BodyColour2));
			}
		};

		struct StreamOutVehicle final : NetworkPacketBase<165> {
			int VehicleID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(VehicleID));
			}
		};

		struct OnPlayerEnterVehicle final : NetworkPacketBase<26> {
			int VehicleID;
			uint8_t Passenger;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ_TYPE(VehicleID, uint16_t, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(Passenger, { NetworkBitStreamValueType::UINT8 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct EnterVehicle final : NetworkPacketBase<26> {
			int PlayerID;
			int VehicleID;
			uint8_t Passenger;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT16(VehicleID));
				bs.write(NetworkBitStreamValue::UINT8(Passenger));
			}
		};

		struct OnPlayerExitVehicle final : NetworkPacketBase<154> {
			int VehicleID;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ_TYPE(VehicleID, uint16_t, { NetworkBitStreamValueType::UINT16 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct ExitVehicle final : NetworkPacketBase<154> {
			int PlayerID;
			int VehicleID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT16(VehicleID));
			}
		};

		struct SetPlayerObjectMaterial final : NetworkPacketBase<84> {
			int ObjectID;
			int MaterialID;
			ObjectMaterialData& MaterialData;

			SetPlayerObjectMaterial(ObjectMaterialData& materialData) : MaterialData(materialData)
			{}

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
				bs.write(NetworkBitStreamValue::UINT8(MaterialData.type));
				bs.write(NetworkBitStreamValue::UINT8(MaterialID));

				if (MaterialData.type == ObjectMaterialData::Type::Default) {
					bs.write(NetworkBitStreamValue::UINT16(MaterialData.model));
					bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(MaterialData.txdOrText));
					bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(MaterialData.textureOrFont));
					bs.write(NetworkBitStreamValue::UINT32(MaterialData.materialColour.ARGB()));
				}
				else if (MaterialData.type == ObjectMaterialData::Type::Text) {
					bs.write(NetworkBitStreamValue::UINT8(MaterialData.materialSize));
					bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(MaterialData.textureOrFont));
					bs.write(NetworkBitStreamValue::UINT8(MaterialData.fontSize));
					bs.write(NetworkBitStreamValue::UINT8(MaterialData.bold));
					bs.write(NetworkBitStreamValue::UINT32(MaterialData.fontColour.ARGB()));
					bs.write(NetworkBitStreamValue::UINT32(MaterialData.backgroundColour.ARGB()));
					bs.write(NetworkBitStreamValue::UINT8(MaterialData.alignment));
					bs.write(NetworkBitStreamValue::COMPRESSED_STR(MaterialData.txdOrText));
				}
			}
		};

		struct CreateObject final : NetworkPacketBase<44> {
			int ObjectID;
			int ModelID;
			Vector3 Position;
			Vector3 Rotation;
			float DrawDistance;
			bool CameraCollision;
			ObjectAttachmentData AttachmentData;
			std::array<ObjectMaterialData, MAX_OBJECT_MATERIAL_SLOTS>& Materials;
			std::bitset<MAX_OBJECT_MATERIAL_SLOTS>& MaterialsUsed;

			CreateObject(
				std::array<ObjectMaterialData, MAX_OBJECT_MATERIAL_SLOTS>& materials,
				std::bitset<MAX_OBJECT_MATERIAL_SLOTS>& materialsUsed
			) :
				Materials(materials),
				MaterialsUsed(materialsUsed)
			{}

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
				bs.write(NetworkBitStreamValue::INT32(ModelID));
				bs.write(NetworkBitStreamValue::VEC3(Position));
				bs.write(NetworkBitStreamValue::VEC3(Rotation));
				bs.write(NetworkBitStreamValue::FLOAT(DrawDistance));
				bs.write(NetworkBitStreamValue::UINT8(CameraCollision));

				bs.write(NetworkBitStreamValue::UINT16(AttachmentData.type == ObjectAttachmentData::Type::Vehicle ? AttachmentData.ID : INVALID_VEHICLE_ID));
				bs.write(NetworkBitStreamValue::UINT16(AttachmentData.type == ObjectAttachmentData::Type::Object ? AttachmentData.ID : INVALID_OBJECT_ID));
				if (AttachmentData.type == ObjectAttachmentData::Type::Vehicle || AttachmentData.type == ObjectAttachmentData::Type::Object) {
					bs.write(NetworkBitStreamValue::VEC3(AttachmentData.offset));
					bs.write(NetworkBitStreamValue::VEC3(AttachmentData.rotation));
					bs.write(NetworkBitStreamValue::UINT8(AttachmentData.syncRotation));
				}

				bs.write(NetworkBitStreamValue::UINT8(MaterialsUsed.count()));
				for (int i = 0; i < MaterialsUsed.count(); ++i) {
					if (MaterialsUsed.test(i)) {
						const ObjectMaterialData& data = Materials[i];
						bs.write(NetworkBitStreamValue::UINT8(data.type));
						bs.write(NetworkBitStreamValue::UINT8(i));

						if (data.type == ObjectMaterialData::Type::Default) {
							bs.write(NetworkBitStreamValue::UINT16(data.model));
							bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(data.txdOrText));
							bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(data.textureOrFont));
							bs.write(NetworkBitStreamValue::UINT32(data.materialColour.ARGB()));
						}
						else if (data.type == ObjectMaterialData::Type::Text) {
							bs.write(NetworkBitStreamValue::UINT8(data.materialSize));
							bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_8(data.textureOrFont));
							bs.write(NetworkBitStreamValue::UINT8(data.fontSize));
							bs.write(NetworkBitStreamValue::UINT8(data.bold));
							bs.write(NetworkBitStreamValue::UINT32(data.fontColour.ARGB()));
							bs.write(NetworkBitStreamValue::UINT32(data.backgroundColour.ARGB()));
							bs.write(NetworkBitStreamValue::UINT8(data.alignment));
							bs.write(NetworkBitStreamValue::COMPRESSED_STR(data.txdOrText));
						}
					}
				}
			}
		};

		struct DestroyObject final : NetworkPacketBase<47> {
			int ObjectID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
			}
		};

		struct MoveObject final : NetworkPacketBase<99> {
			int ObjectID;
			Vector3 CurrentPosition;
			ObjectMoveData MoveData;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
				bs.write(NetworkBitStreamValue::VEC3(CurrentPosition));
				bs.write(NetworkBitStreamValue::VEC3(MoveData.targetPos));
				bs.write(NetworkBitStreamValue::FLOAT(MoveData.speed));
				bs.write(NetworkBitStreamValue::VEC3(MoveData.targetRot));
			}
		};

		struct StopObject final : NetworkPacketBase<122> {
			int ObjectID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
			}
		};

		struct SetObjectPosition final : NetworkPacketBase<45> {
			int ObjectID;
			Vector3 Position;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
				bs.write(NetworkBitStreamValue::VEC3(Position));
			}
		};

		struct SetObjectRotation final : NetworkPacketBase<46> {
			int ObjectID;
			Vector3 Rotation;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
				bs.write(NetworkBitStreamValue::VEC3(Rotation));
			}
		};

		struct AttachObjectToPlayer final : NetworkPacketBase<75> {
			int ObjectID;
			int PlayerID;
			Vector3 Offset;
			Vector3 Rotation;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::VEC3(Offset));
				bs.write(NetworkBitStreamValue::VEC3(Rotation));
			}
		};

		struct SetPlayerAttachedObject final : NetworkPacketBase<113> {
			int PlayerID;
			int Index;
			bool Create;
			ObjectAttachmentSlotData AttachmentData;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerID));
				bs.write(NetworkBitStreamValue::UINT32(Index));
				bs.write(NetworkBitStreamValue::BIT(Create));
				if (Create) {
					bs.write(NetworkBitStreamValue::UINT32(AttachmentData.model));
					bs.write(NetworkBitStreamValue::UINT32(AttachmentData.bone));
					bs.write(NetworkBitStreamValue::VEC3(AttachmentData.offset));
					bs.write(NetworkBitStreamValue::VEC3(AttachmentData.rotation));
					bs.write(NetworkBitStreamValue::VEC3(AttachmentData.scale));
					bs.write(NetworkBitStreamValue::UINT32(AttachmentData.colour1.ARGB()));
					bs.write(NetworkBitStreamValue::UINT32(AttachmentData.colour2.ARGB()));
				}
			}
		};

		struct PlayerBeginObjectSelect final : NetworkPacketBase<27> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct OnPlayerSelectObject final : NetworkPacketBase<27> {
			int SelectType;
			int ObjectID;
			int Model;
			Vector3 Position;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ_TYPE(SelectType, uint32_t, { NetworkBitStreamValueType::UINT32 });
				CHECKED_READ_TYPE(ObjectID, uint16_t, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ_TYPE(Model, uint32_t, { NetworkBitStreamValueType::UINT32 });
				CHECKED_READ(Position, { NetworkBitStreamValueType::VEC3 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerCancelObjectEdit final : NetworkPacketBase<28> {
			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerBeginObjectEdit final : NetworkPacketBase<117> {
			bool PlayerObject;
			int ObjectID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::BIT(PlayerObject));
				bs.write(NetworkBitStreamValue::UINT16(ObjectID));
			}
		};

		struct OnPlayerEditObject final : NetworkPacketBase<117> {
			bool PlayerObject;
			int ObjectID;
			int Response;
			Vector3 Offset;
			Vector3 Rotation;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(PlayerObject, { NetworkBitStreamValueType::BIT });
				CHECKED_READ_TYPE(ObjectID, uint16_t, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ_TYPE(Response, uint32_t, { NetworkBitStreamValueType::UINT32 });
				CHECKED_READ(Offset, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(Rotation, { NetworkBitStreamValueType::VEC3 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerBeginAttachedObjectEdit final : NetworkPacketBase<116> {
			unsigned Index;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT32(Index));
			}
		};

		struct OnPlayerEditAttachedObject final : NetworkPacketBase<116> {
			unsigned Response;
			unsigned Index;
			ObjectAttachmentSlotData AttachmentData;

			bool read(INetworkBitStream& bs) {
				bs.readT<uint32_t>(Response, NetworkBitStreamValueType::UINT32);
				bs.readT<uint32_t>(Index, NetworkBitStreamValueType::UINT32);
				bs.readT<uint32_t>(AttachmentData.model, NetworkBitStreamValueType::UINT32);
				bs.readT<uint32_t>(AttachmentData.bone, NetworkBitStreamValueType::UINT32);
				bs.read(AttachmentData.offset, NetworkBitStreamValueType::VEC3);
				bs.read(AttachmentData.rotation, NetworkBitStreamValueType::VEC3);
				bs.read(AttachmentData.scale, NetworkBitStreamValueType::VEC3);
				uint32_t argb;
				bs.read(argb, NetworkBitStreamValueType::UINT32);
				AttachmentData.colour1 = Colour::FromARGB(argb);
				bool res = bs.read(argb, NetworkBitStreamValueType::UINT32);
				AttachmentData.colour2 = Colour::FromARGB(argb);
				return res;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerShowTextLabel final : NetworkPacketBase<36> {
			bool PlayerTextLabel;
			int TextLabelID;
			Colour Col;
			Vector3 Position;
			float DrawDistance;
			bool LOS;
			int PlayerAttachID;
			int VehicleAttachID;
			NetworkString Text;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerTextLabel ? TEXT_LABEL_POOL_SIZE + TextLabelID : TextLabelID));
				bs.write(NetworkBitStreamValue::UINT32(Col.RGBA()));
				bs.write(NetworkBitStreamValue::VEC3(Position));
				bs.write(NetworkBitStreamValue::FLOAT(DrawDistance));
				bs.write(NetworkBitStreamValue::UINT8(LOS));
				bs.write(NetworkBitStreamValue::UINT16(PlayerAttachID));
				bs.write(NetworkBitStreamValue::UINT16(VehicleAttachID));
				bs.write(NetworkBitStreamValue::COMPRESSED_STR(Text));
			}
		};

		struct PlayerHideTextLabel final : NetworkPacketBase<58> {
			bool PlayerTextLabel;
			int TextLabelID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerTextLabel ? TEXT_LABEL_POOL_SIZE + TextLabelID : TextLabelID));
			}
		};

		struct PlayerCreatePickup final : NetworkPacketBase<95> {
			int PickupID;
			int Model;
			int Type;
			Vector3 Position;

			bool read(INetworkBitStream & bs) {
				return false;
			}

			void write(INetworkBitStream & bs) const {
				bs.write(NetworkBitStreamValue::INT32(PickupID));
				bs.write(NetworkBitStreamValue::INT32(Model));
				bs.write(NetworkBitStreamValue::INT32(Type));
				bs.write(NetworkBitStreamValue::VEC3(Position));
			}
		};

		struct PlayerDestroyPickup final : NetworkPacketBase<63> {
			int PickupID;

			bool read(INetworkBitStream & bs) {
				return false;
			}

			void write(INetworkBitStream & bs) const {
				bs.write(NetworkBitStreamValue::INT32(PickupID));
			}
		};

		struct OnPlayerPickUpPickup final : NetworkPacketBase<131> {
			int PickupID;

			bool read(INetworkBitStream & bs) {
				return bs.readT<int32_t>(PickupID, NetworkBitStreamValueType::INT32);
			}

			void write(INetworkBitStream & bs) const {
			}
		};

		struct PlayerShowTextDraw final : NetworkPacketBase<134> {
			bool PlayerTextDraw;
			int TextDrawID;
			bool UseBox;
			bool Alignment;
			bool Proportional;
			Vector2 LetterSize;
			Colour LetterColour;
			Vector2 TextSize;
			Colour BoxColour;
			int Shadow;
			int Outline;
			Colour BackColour;
			int Style;
			bool Selectable;
			Vector2 Position;
			int Model;
			Vector3 Rotation;
			float Zoom;
			int Color1;
			int Color2;
			NetworkString Text;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				uint8_t flags = UseBox | (Alignment << 1) | (Proportional << 4);
				bs.write(NetworkBitStreamValue::UINT16(PlayerTextDraw ? GLOBAL_TEXTDRAW_POOL_SIZE + TextDrawID : TextDrawID));
				bs.write(NetworkBitStreamValue::UINT8(flags));
				bs.write(NetworkBitStreamValue::VEC2(LetterSize));
				bs.write(NetworkBitStreamValue::UINT32(LetterColour.ABGR()));
				bs.write(NetworkBitStreamValue::VEC2(TextSize));
				bs.write(NetworkBitStreamValue::UINT32(BoxColour.ABGR()));
				bs.write(NetworkBitStreamValue::UINT8(Shadow));
				bs.write(NetworkBitStreamValue::UINT8(Outline));
				bs.write(NetworkBitStreamValue::UINT32(BackColour.ABGR()));
				bs.write(NetworkBitStreamValue::UINT8(Style));
				bs.write(NetworkBitStreamValue::UINT8(Selectable));
				bs.write(NetworkBitStreamValue::VEC2(Position));
				bs.write(NetworkBitStreamValue::UINT16(Model));
				bs.write(NetworkBitStreamValue::VEC3(Rotation));
				bs.write(NetworkBitStreamValue::FLOAT(Zoom));
				bs.write(NetworkBitStreamValue::INT16(Color1));
				bs.write(NetworkBitStreamValue::INT16(Color2));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_16(Text));
			}
		};

		struct PlayerHideTextDraw final : NetworkPacketBase<135> {
			bool PlayerTextDraw;
			int TextDrawID;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerTextDraw ? GLOBAL_TEXTDRAW_POOL_SIZE + TextDrawID : TextDrawID));
			}
		};

		struct PlayerTextDrawSetString final : NetworkPacketBase<105> {
			bool PlayerTextDraw;
			int TextDrawID;
			NetworkString Text;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT16(PlayerTextDraw ? GLOBAL_TEXTDRAW_POOL_SIZE + TextDrawID : TextDrawID));
				bs.write(NetworkBitStreamValue::DYNAMIC_LEN_STR_16(Text));
			}
		};

		struct PlayerBeginTextDrawSelect final : NetworkPacketBase<83> {
			Colour Col;
			bool Enable;

			bool read(INetworkBitStream& bs) {
				return false;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::BIT(Enable));
				bs.write(NetworkBitStreamValue::UINT32(Col.RGBA()));
			}
		};

		struct OnPlayerSelectTextDraw final : NetworkPacketBase<83> {
			bool PlayerTextDraw;
			bool Invalid;
			int TextDrawID;

			bool read(INetworkBitStream& bs) {
				bool res = bs.readT<uint16_t>(TextDrawID, NetworkBitStreamValueType::UINT16);
				Invalid = TextDrawID == INVALID_TEXTDRAW;
				if (!Invalid) {
					PlayerTextDraw = TextDrawID >= GLOBAL_TEXTDRAW_POOL_SIZE;
					if (PlayerTextDraw) {
						TextDrawID -= GLOBAL_TEXTDRAW_POOL_SIZE;
					}
				}
				return res;
			}

			void write(INetworkBitStream& bs) const {
			}
		};

		struct PlayerInitMenu final : NetworkPacketBase<76> {
			uint8_t MenuID;
			bool HasTwoColumns;
			NetworkString Title;
			Vector2 Position;
			float Col1Width;
			float Col2Width;
			bool MenuEnabled;
			std::array<bool, MAX_MENU_ITEMS> RowEnabled;
			std::array<NetworkString, 2> ColumnHeaders;
			std::array<uint8_t, 2> ColumnItemCount;
			std::array<std::array<NetworkString, MAX_MENU_ITEMS>, 2> MenuItems;

			bool read(INetworkBitStream & bs) {
				return false;
			}

			void write(INetworkBitStream & bs) const {
				bs.write(NetworkBitStreamValue::UINT8(MenuID));
				bs.write(NetworkBitStreamValue::UINT32(HasTwoColumns));

				// Menu title is a fixed size string
				String menuTitleFixed(Title);
				menuTitleFixed.resize(MAX_MENU_TEXT_LENGTH);
				bs.write(NetworkBitStreamValue::FIXED_LEN_STR(NetworkString(menuTitleFixed)));

				bs.write(NetworkBitStreamValue::VEC2(Position));
				bs.write(NetworkBitStreamValue::FLOAT(Col1Width));

				// Only send this when menu has two columns
				if (HasTwoColumns) {
					bs.write(NetworkBitStreamValue::FLOAT(Col2Width));
				}

				bs.write(NetworkBitStreamValue::UINT32(MenuEnabled));
				for (bool isRowEnabled : RowEnabled) {
					bs.write(NetworkBitStreamValue::UINT32(isRowEnabled));
				}

				// Get first column data
				uint8_t firstColumnItemCount = ColumnItemCount.at(0);
				NetworkString firstColumnHeader = ColumnHeaders.at(0);
				std::array<NetworkString, MAX_MENU_ITEMS> firstColumnItems = MenuItems.at(0);

				// Send first column header as a fixed string
				String firstColumnHeaderFixed(firstColumnHeader);
				firstColumnHeaderFixed.resize(MAX_MENU_TEXT_LENGTH);
				bs.write(NetworkBitStreamValue::FIXED_LEN_STR(NetworkString(firstColumnHeaderFixed)));

				bs.write(NetworkBitStreamValue::UINT8(firstColumnItemCount));
				for (uint8_t i = 0; i < firstColumnItemCount; i++) {
					// Send items/rows of first column as fixed size string
					String itemTextFixed(firstColumnItems.at(i));
					itemTextFixed.resize(MAX_MENU_TEXT_LENGTH);
					bs.write(NetworkBitStreamValue::FIXED_LEN_STR(NetworkString(itemTextFixed)));
				}

				if (HasTwoColumns) {
					// Get second column data
					uint8_t secondColumnItemCount = ColumnItemCount.at(1);
					NetworkString secondColumnHeader = ColumnHeaders.at(1);
					std::array<NetworkString, MAX_MENU_ITEMS> secondColumnItems = MenuItems.at(1);

					// Send second second header as a fixed string
					String secondColumnHeaderFixed(secondColumnHeader);
					secondColumnHeaderFixed.resize(MAX_MENU_TEXT_LENGTH);
					bs.write(NetworkBitStreamValue::FIXED_LEN_STR(NetworkString(secondColumnHeaderFixed)));

					bs.write(NetworkBitStreamValue::UINT8(secondColumnItemCount));
					for (uint8_t i = 0; i < secondColumnItemCount; i++) {
						// Send items/rows of second column as fixed size string
						String itemTextFixed(secondColumnItems.at(i));
						itemTextFixed.resize(MAX_MENU_TEXT_LENGTH);
						bs.write(NetworkBitStreamValue::FIXED_LEN_STR(NetworkString(itemTextFixed)));
					}
				}
			}
		};
	
		struct PlayerShowMenu final : NetworkPacketBase<77> {
			uint8_t MenuID;

			bool read(INetworkBitStream & bs) {
				return false;
			}

			void write(INetworkBitStream & bs) const {
				bs.write(NetworkBitStreamValue::UINT8(MenuID));
			}
		};

		struct PlayerHideMenu final : NetworkPacketBase<78> {
			uint8_t MenuID;

			bool read(INetworkBitStream & bs) {
				return false;
			}

			void write(INetworkBitStream & bs) const {
				bs.write(NetworkBitStreamValue::UINT8(MenuID));
			}
		};

		struct OnPlayerSelectedMenuRow final : NetworkPacketBase<132> {
			uint8_t MenuRow;

			bool read(INetworkBitStream & bs) {
				return bs.readT<uint8_t>(MenuRow, NetworkBitStreamValueType::UINT8);
			}

			void write(INetworkBitStream & bs) const {
			}
		};

		struct OnPlayerExitedMenu final : NetworkPacketBase<140> {
			bool read(INetworkBitStream & bs) {
				return true;
			}

			void write(INetworkBitStream & bs) const {
			}
		};
	}
	namespace Packet {
		struct PlayerFootSync final : NetworkPacketBase<207> {
			int PlayerID;
			uint16_t LeftRight;
			uint16_t UpDown;
			uint16_t Keys;
			Vector3 Position;
			GTAQuat Rotation;
			Vector2 HealthArmour;
			uint8_t Weapon;
			uint8_t SpecialAction;
			Vector3 Velocity;
			uint16_t AnimationID;
			uint16_t AnimationFlags;
			PlayerSurfingData SurfingData;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(LeftRight, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(UpDown, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(Keys, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(Position, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(Rotation, {NetworkBitStreamValueType::GTA_QUAT });
				CHECKED_READ(HealthArmour, { NetworkBitStreamValueType::HP_ARMOR_COMPRESSED });
				CHECKED_READ(Weapon, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(SpecialAction, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(Velocity, { NetworkBitStreamValueType::VEC3 });
				if (!bs.read(SurfingData.offset, NetworkBitStreamValueType::VEC3)) {
					return false;
				}
				uint16_t surfingID;
				CHECKED_READ(surfingID, { NetworkBitStreamValueType::UINT16 });
				SurfingData.ID = surfingID;
				if (SurfingData.ID < VEHICLE_POOL_SIZE) {
					SurfingData.type = PlayerSurfingData::Type::Vehicle;
				}
				else if (SurfingData.ID < VEHICLE_POOL_SIZE +OBJECT_POOL_SIZE) {
					SurfingData.ID -= VEHICLE_POOL_SIZE;
					SurfingData.type = PlayerSurfingData::Type::Object;
				}
				else {
					SurfingData.type = PlayerSurfingData::Type::None;
				}
				CHECKED_READ(AnimationID, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(AnimationFlags, { NetworkBitStreamValueType::UINT16 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(getID(bs.getNetworkType())));
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(PlayerID)));

				bs.write(NetworkBitStreamValue::BIT(LeftRight > 0));
				if (LeftRight) {
					bs.write(NetworkBitStreamValue::UINT16(LeftRight));
				}

				bs.write(NetworkBitStreamValue::BIT(UpDown > 0));
				if (UpDown) {
					bs.write(NetworkBitStreamValue::UINT16(UpDown));
				}

				bs.write(NetworkBitStreamValue::UINT16(Keys));
				bs.write(NetworkBitStreamValue::VEC3(Position));
				bs.write(NetworkBitStreamValue::GTA_QUAT(Rotation));
				bs.write(NetworkBitStreamValue::HP_ARMOR_COMPRESSED(HealthArmour));
				bs.write(NetworkBitStreamValue::UINT8(Weapon));
				bs.write(NetworkBitStreamValue::UINT8(SpecialAction));
				bs.write(NetworkBitStreamValue::VEC3_SAMP(Velocity));

				bs.write(NetworkBitStreamValue::BIT(SurfingData.type != PlayerSurfingData::Type::None));
				if (SurfingData.type != PlayerSurfingData::Type::None) {
					int id = 0;
					if (SurfingData.type == PlayerSurfingData::Type::Vehicle) {
						id = SurfingData.ID;
					}
					else if (SurfingData.type == PlayerSurfingData::Type::Object) {
						id = SurfingData.ID + VEHICLE_POOL_SIZE;
					}
					bs.write(NetworkBitStreamValue::UINT16(id));
					bs.write(NetworkBitStreamValue::VEC3(SurfingData.offset));
				}

				bs.write(NetworkBitStreamValue::BIT(AnimationID > 0));
				if (AnimationID) {
					bs.write(NetworkBitStreamValue::UINT16(AnimationID));
					bs.write(NetworkBitStreamValue::UINT16(AnimationFlags));
				}
			}
		};

		struct PlayerAimSync final : NetworkPacketBase<203> {
			int PlayerID;
			uint8_t CamMode;
			Vector3 CamFrontVector;
			Vector3 CamPos;
			float AimZ;
			union {
				uint8_t ZoomWepState;
				struct {
					uint8_t CamZoom : 6;
					uint8_t WeaponState : 2;
				};
			};
			uint8_t AspectRatio;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(CamMode, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(CamFrontVector, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(CamPos, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(AimZ, { NetworkBitStreamValueType::FLOAT });
				CHECKED_READ(ZoomWepState, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(AspectRatio, { NetworkBitStreamValueType::UINT8 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(getID(bs.getNetworkType())));
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(PlayerID)));
				bs.write(NetworkBitStreamValue::UINT8(CamMode));
				bs.write(NetworkBitStreamValue::VEC3(CamFrontVector));
				bs.write(NetworkBitStreamValue::VEC3(CamPos));
				bs.write(NetworkBitStreamValue::FLOAT(AimZ));
				bs.write(NetworkBitStreamValue::UINT8(ZoomWepState));
				bs.write(NetworkBitStreamValue::UINT8(AspectRatio));
			}
		};

		struct PlayerBulletSync final : NetworkPacketBase<206> {
			int PlayerID;
			uint8_t HitType;
			uint16_t HitID;
			Vector3 Origin;
			Vector3 HitPos;
			Vector3 Offset;
			uint8_t WeaponID;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(HitType, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(HitID, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(Origin, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(HitPos, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(Offset, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(WeaponID, { NetworkBitStreamValueType::UINT8 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(getID(bs.getNetworkType())));
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(PlayerID)));
				bs.write(NetworkBitStreamValue::UINT8(HitType));
				bs.write(NetworkBitStreamValue::UINT16(HitID));
				bs.write(NetworkBitStreamValue::VEC3(Origin));
				bs.write(NetworkBitStreamValue::VEC3(HitPos));
				bs.write(NetworkBitStreamValue::VEC3(Offset));
				bs.write(NetworkBitStreamValue::UINT8(WeaponID));
			}
		};

		struct PlayerStatsSync final : NetworkPacketBase<205> {
			int32_t Money;
			int32_t DrunkLevel;

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(Money, { NetworkBitStreamValueType::INT32 });
				CHECKED_READ(DrunkLevel, { NetworkBitStreamValueType::INT32 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::INT32(Money));
				bs.write(NetworkBitStreamValue::INT32(DrunkLevel));
			}
		};

		struct PlayerMarkersSync final : NetworkPacketBase<208> {
		};

		struct PlayerVehicleSync final : NetworkPacketBase<200> {
			int PlayerID;
			uint16_t VehicleID;
			uint16_t LeftRight;
			uint16_t UpDown;
			uint16_t Keys;
			GTAQuat Rotation;
			Vector3 Position;
			Vector3 Velocity;
			float Health;
			Vector2 PlayerHealthArmour;
			uint8_t Siren;
			uint8_t LandingGear;
			uint16_t TrailerID;
			bool HasTrailer;

			union {
				uint8_t AdditionalKeyWeapon;
				struct {
					uint8_t WeaponID : 6;
					uint8_t AdditionalKey : 2;
				};
			};

			union {
				uint32_t AbysmalShit;
				struct {
					uint32_t			 HydraThrustAngle;
					float				 TrainSpeed;
				};
			};

			bool read(INetworkBitStream& bs) {
				CHECKED_READ(VehicleID, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(LeftRight, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(UpDown, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(Keys, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(Rotation, { NetworkBitStreamValueType::GTA_QUAT });
				CHECKED_READ(Position, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(Velocity, { NetworkBitStreamValueType::VEC3 });
				CHECKED_READ(Health, { NetworkBitStreamValueType::FLOAT });
				CHECKED_READ(PlayerHealthArmour, { NetworkBitStreamValueType::HP_ARMOR_COMPRESSED });
				CHECKED_READ(AdditionalKeyWeapon, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(Siren, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(LandingGear, { NetworkBitStreamValueType::UINT8 });
				CHECKED_READ(TrailerID, { NetworkBitStreamValueType::UINT16 });
				CHECKED_READ(AbysmalShit, { NetworkBitStreamValueType::UINT32 });
				return true;
			}

			void write(INetworkBitStream& bs) const {
				bs.write(NetworkBitStreamValue::UINT8(getID(bs.getNetworkType())));
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(PlayerID)));
				bs.write(NetworkBitStreamValue::UINT16(VehicleID));
				bs.write(NetworkBitStreamValue::UINT16(LeftRight));
				bs.write(NetworkBitStreamValue::UINT16(UpDown));
				bs.write(NetworkBitStreamValue::UINT16(Keys));
				bs.write(NetworkBitStreamValue::GTA_QUAT(Rotation));
				bs.write(NetworkBitStreamValue::VEC3(Position));
				bs.write(NetworkBitStreamValue::VEC3_SAMP(Velocity));
				bs.write(NetworkBitStreamValue::UINT16(uint16_t(Health)));
				bs.write(NetworkBitStreamValue::HP_ARMOR_COMPRESSED(PlayerHealthArmour));
				bs.write(NetworkBitStreamValue::UINT8(AdditionalKeyWeapon));
				bs.write(NetworkBitStreamValue::BIT(Siren));
				bs.write(NetworkBitStreamValue::BIT(LandingGear));

				bs.write(NetworkBitStreamValue::BIT(AbysmalShit > 0));
				if (AbysmalShit) {
					bs.write(NetworkBitStreamValue::UINT32(AbysmalShit));
				}

				bs.write(NetworkBitStreamValue::BIT(HasTrailer));
				if (HasTrailer) {
					bs.write(NetworkBitStreamValue::UINT16(TrailerID));
				}
			}
		};
	}
}