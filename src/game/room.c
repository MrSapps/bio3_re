#include "pl_wep.h"
#include "sca.h"
#include "monitor.h"
#include "pl_wep2.h"
#include "pad.h"
#include "sce0.h"
#include "esp.h"

ESP_WORK *func_8001B484(ULONG Id, ULONG Dir, MATRIX *pM, SVECTOR *pV, SVECTOR *pDsv);
SHORT func_8006D030(ULONG Arg0);
void func_80044804(MATRIX *pM, ULONG Arg1, ULONG Type);
void func_80047860(MATRIX *pM, ULONG Arg1, ULONG Type);
extern ULONG D_8009DBB4[];
ULONG func_800102E8(void);                 /* rand */
SHORT Get_dir_xz(LONG X, LONG Z);
LONG func_800455B4();
LONG func_8004EF74(ULONG P0, ULONG P1, ULONG P2, ULONG P3);

extern UCHAR D_8009CD3D;
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
signed char func_8006CF0C_r(ULONG Arg0) __asm__("func_8006CF0C");
void Pl_water(ENEMY_WORK *pEm);

/* declarations from earlier parts of the original address-split file */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
void func_8006CF0C(ULONG Arg0);
void Mag_set(PLAYER_WORK *pEm);

void Pl_wep_flag_set(PLAYER_WORK *pEm);

#define RND() ((signed char)func_800102E8())

extern PARTS_W *D_8009CD30;
extern USHORT D_8009DCB4[];
void Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
void func_8007809C(LONG a0, LONG a1, ULONG a2, ULONG a3);

extern ENEMY_WORK Pl_work;
extern UCHAR D_8009DDC4[][2];
extern UCHAR D_8009DE10;
extern UCHAR D_8009DE24;

extern UCHAR D_8009DE24;
extern USHORT D_8009DE26;
extern signed char D_8009DE28;

extern LONG D_800D204C[];

void func_80010150(void *p, ULONG Size);
void func_80048FF4(void);

#define PLAYER ((ENEMY_WORK *)((UCHAR *)&G + 0x248C))

extern UCHAR D_800D225E;
extern UCHAR D_800E017F;
extern UCHAR D_800E0186[];
extern ULONG D_80098700;
extern USHORT *D_8009DFD0[];           /* per stage room file numbers */
LONG Cd_read_l(LONG Fno, ULONG *pAdr, LONG a2, char *pName) __asm__("Cd_read");
void Kage_work_init(void);
void Kage_work9_init(void);
void func_80077ED4(void);
void Esp_init_R(void);
void func_80078204(void);
void func_80033508(void);
void func_800486C8(void);
void Sce_model_init(void);
void func_80037E80(void);
void Sce_scheduler_set(void);
void func_800498F0(void);
void func_80048630(void);
void func_80078450(void);
void Em_init_move(void);
void func_80071510(LONG Stage, LONG Room);
void func_8006ECB0(LONG Id);
void Player_set(ENEMY_WORK *pEm);

/* load and set up the room (RE2 Set_room) */
void Set_room(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pPl = PLAYER;
    ROOM_WORK *pRoom;
    ENEMY_WORK **ppEm;
    ULONG **ppAdr;
    LONG Stage;
    LONG Room;
    LONG Size;
    LONG i;
    ENEMY_WORK *pEm_mem;

    pEm_mem = (ENEMY_WORK *)pG->Unk2138;
    pG->Ctp.Tpage_clut = 0xD05;
    Stage = pG->Unk783E;
    Room = pG->Unk7840;
    pG->Unk75EA = 0;
    pG->Unk75E8 = 0;
    pG->pEm_mem = pEm_mem;
    pG->pRoom = (ROOM_WORK *)pEm_mem;
    pG->Status_flg &= 0xFFFF0000;
    pRoom = pG->pRoom;
    Kage_work_init();
    Kage_work9_init();
    func_80010150(pG->pad2708, 0x1FC);
    ppEm = &pG->pEnemy[1];
    pG->ppEnemy_end = &pG->pEnemy[2];
    pG->Em_num = 0;
    for (i = 41; i > 0; i--) {
        *ppEm++ = (ENEMY_WORK *)pG->pad2708;
    }
    pG->Em_bin_info[0].Id = 0xFF;
    pG->Em_bin_info[1].Id = 0xFF;
    pG->Em_bin_info[0].Use = 0;
    pG->Em_bin_info[1].Use = 0;
    pG->pObj_work_end = &pG->Ob_model[0];
    for (i = 0; i < 32; i++) {
        pG->Ob_model[0].Be_flg = 0;
    }
    if (pPl->Id != pG->Unk784E) {
        if (pG->Unk784E >= 8) {
            if (pPl->Id < 8) {
                pPl->Life = 200;
                pPl->Status_flg = 0;
                pG->Unk7864 = pPl->Id;
            }
            G.Status_flg |= 0x8000000;
            func_8007809C(0, 8, 0x801FBC00, 0x801C1814);
        } else {
            if (pPl->Id >= 8) {
                pPl->Life = 200;
                pPl->Status_flg = 0;
                pG->Unk784E = pG->Unk7864;
            } else {
                if (pG->Unk784E == 1) {
                    D_800D225E = 10;
                }
                if (pG->Unk7864 >= 2) {
                    if (pG->Unk784E < 2) {
                        pG->Unk784E = pG->Unk7864;
                    }
                } else if (pG->Unk784E >= 2) {
                    pG->Unk7864 = pG->Unk784E;
                }
            }
            G.Status_flg &= ~0x8000000;
            func_8007809C(0, 2, 0x801FBC00, 0x801C1814);
        }
        pPl->Max_life = 200;
        pPl->Id = pG->Unk784E;
        func_8006ECB0(pG->Unk784E);
        Player_set(pPl);
        *(ULONG *)&pPl->Routine_0 = 0;
    }
    func_80077ED4();
    Size = Cd_read_l(D_8009DFD0[Stage][Room], NULL, 4, "ARD");
    ppAdr = &pRoom->pEdt0;
    do {
        if (*ppAdr) {
            *ppAdr = (ULONG *)((UCHAR *)*ppAdr + (ULONG)pRoom);
        }
        ppAdr++;
    } while (ppAdr < (ULONG **)pRoom->Cut);
    for (i = 0; i < pRoom->nCut; i++) {
        pRoom->pRcut[i].pSp = (ULONG *)((UCHAR *)pRoom->pRcut[i].pSp + (ULONG)pRoom);
    }
    pG->pEm_mem = (ENEMY_WORK *)((UCHAR *)pG->pEm_mem + Size);
    for (i = 0; i < pRoom->nOmodel; i++) {
        pRoom->pOmodel[i].Tim_adr = (ULONG *)((UCHAR *)pRoom->pOmodel[i].Tim_adr + (ULONG)pRoom);
        pRoom->pOmodel[i].Tmd_adr = (ULONG *)((UCHAR *)pRoom->pOmodel[i].Tmd_adr + (ULONG)pRoom);
    }
    Esp_init_R();
    func_80078204();
    Cut_change(pG->Cut_no);
    func_80033508();
    func_800486C8();
    Sce_model_init();
    func_80037E80();
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
    }
    Sce_scheduler_set();
    if (G.Em_num != 0 || D_800E017F != 0xFF || G.pEnemy[1]->Be_flg != 0) {
        Cd_read_l(D_8009DFD0[Stage][Room] + 1, NULL, 5, "BIN FILE");
    }
    func_800498F0();
    func_80048630();
    func_80078450();
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
    }
    Em_init_move();
    func_80071510(Stage, Room);
}

void Set_stage(void)
{
    ULONG Size = ((ULONG (*)(ULONG, ULONG))Task_lexecute)(2, G.Unk783E + 0x11);

    G.Unk2138 = ((Size + 3) & ~3) + 0x8011A000;
    Task_sleep(1);
}

/* hand the room's sub animation sets (pRoom->pEdd) to the player and enemy works */
void func_800498F0(void)
{
    UCHAR *pEdd;
    ULONG *pOfs;
    ULONG *pKan;
    ULONG Bits;
    UCHAR No;
    UCHAR Cnt;
    ENEMY_WORK *pEm;

    No = 0;
    if (D_800E017F == 0xFF) {
        return;
    }
    pEdd = G.pRoom->pEdd;
    Cnt = pEdd[4];
    pOfs = (ULONG *)(pEdd + *(ULONG *)pEdd);
    do {
        pKan = (ULONG *)(pEdd + pOfs[0]);
        Bits = *pKan;
        if (Bits & 1) {
            G.Pl_pUnk100_kan_t_ptr = pKan + 1;
            G.Pl_pUnk104_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
        }
        for (Bits >>= 1; Bits != 0; Bits >>= 1, No++) {
            if (Bits & 1) {
                if (No < 15) {
                    pEm = G.pEnemy[No + 1];
                    if (pEm->Be_flg & 1) {
                        pEm->pUnk100_kan_t_ptr = pKan + 1;
                        G.pEnemy[No + 1]->pUnk104_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
                    }
                } else if (No < 23) {
                    G.pEnemy[No - 15]->pUnk19C_kan_t_ptr = pKan + 1;
                    G.pEnemy[No - 15]->pUnk198_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
                } else {
                    G.pEnemy[No - 23]->pUnk194_kan_t_ptr = pKan + 1;
                    G.pEnemy[No - 23]->pUnk190_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
                }
            }
        }
        pOfs += 2;
        No = 0;
    } while (--Cnt);
}

void func_80038F98(ENEMY_WORK *pEm);

/* run every enemy's routine once at room start (RE2 Em_init_move) */
void Em_init_move(void)
{
    LONG Stop_save;
    LONG Timer0_save;
    ULONG Type_hi;
    ENEMY_WORK *pEm;
    ENEMY_WORK **ppEm;

    Type_hi = 0;
    if (G.Unk2468 & 0x20000000) {
        G.Unk2468 &= ~0x20000000;
        Stop_save = 1;
    } else {
        Stop_save = 0;
    }
    func_80038F98(PLAYER);
    ppEm = &G.pEnemy[1];
    if (G.ppEnemy_end != ppEm) {
        do {
            pEm = *ppEm;
            G.pEm = pEm;
            if (pEm->Be_flg & 1) {
                Timer0_save = pEm->Timer0;
                G.Em_routine[pEm->Id - 0x10](pEm);
                if (Timer0_save != 0) {
                    *(ULONG *)&pEm->Routine_0 = ((Timer0_save & 0xFF00) - 0x100) | 4;
                    pEm->Move_no = Timer0_save;
                    pEm->Move_cnt = 0;
                    pEm->Unk144 = pEm->UnkDA;
                    G.Em_routine[pEm->Id - 0x10](pEm);
                }
                pEm->Type |= Type_hi; /* RE2 restored Type & 0x8000 here; RE3 keeps a dead 0 */
            }
            ppEm++;
        } while (ppEm != G.ppEnemy_end);
    }
    if (Stop_save) {
        G.Unk2468 |= 0x20000000;
    }
}
