#include "enemy/em22.h"
#include "door.h"
#include "joint.h"
#include "root_ck.h"
#include "snd.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
LONG Guide_check_(LONG X, LONG Z, LONG Floor) __asm__("Guide_check"); /* arg 1 long (canonical short); arg 2 long (canonical short); arg 3 long (canonical UC) */
ULONG func_8004D6B8_(LONG X, LONG Z, LONG a2, LONG a3) __asm__("func_8004D6B8"); /* 4 args here (canonical 2) */
void func_8002646C_(ENEMY_WORK *pEm, LONG A, LONG B, LONG C) __asm__("Em_tilt_set"); /* arg 2 long (canonical short); arg 3 long (canonical short); arg 4 long (canonical UC) */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector"); /* arg 1 long (canonical short) */

/* RE2 EM_MAIN */
void func_801000BC(ENEMY_WORK *pEm)
{
    LONG Unused[2];

    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em22_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    pEm->Col.Unk26 = pEm->pSin_parts_ptr->Pos_y + 800;
    if (pEm->Col.Unk26 > 0)
        pEm->Col.Unk26 = 0;
    if (EM22_W(pEm)->Unk13) {
        if (EM22_W(pEm)->Unk08) {
            pEm->Col.W0 = pEm->Col.D0 = 600;
        } else if (pEm->Col.W0 < 800) {
            pEm->Col.D0 = pEm->Col.W0 = pEm->Col.W0 + 50;
        }
    } else if (pEm->Col.W0 < 600) {
        pEm->Col.D0 = pEm->Col.W0 = pEm->Col.W0 + 50;
    }
    if (pEm->Water) {
        if (pEm->Water < pEm->pSin_parts_ptr[0].Workm.t[1] && (pEm->Move_cnt & 0xF) == 1 && pEm->Spd_x >= 11)
            func_80108AAC(pEm);
        else if (pEm->Water < pEm->pSin_parts_ptr[16].Workm.t[1] && (pEm->Move_cnt & 0xF) == 1 && pEm->Spd_x >= 11)
            func_80108AAC(pEm);
    }
}

/* RE2 Em_init (Em22_Init) */
void func_8010023C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    COL_OBJ *pCol;

    EM_SET_MOVE(pEm, 0, 0, 0xF);
    EM_SET_RTN(pEm, 1, 0, 0, 0);
    Kage_work_set(&pEm->Kage.pKw, 0, 0x3E803E8, 0, pEm->pSin_parts_ptr[1].Workm.t);
    pCol = &pEm->Col;
    pEm->Hit_area.Top = 1600;
    pEm->Hit_area.Bottom = -1000;
    pEm->Hit_area.Rate0 = 0x78;
    pEm->Hit_area.Rate1 = 0xBC;
    pEm->Hit_area.R = 600;
    pEm->Unk140 = 0;
    pEm->Unk16B = 0;
    pEm->Unk16A = 0;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr[2].Workm;
    pCol->W0 = pCol->D0 = 800;
    pCol->H = 2000;
    pCol->Unk24 = 500;
    pCol->pEm = pEm;
    pCol->Unk26 = pCol->Unk28 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    func_80033580(pCol, 0);
    pP = &pEm->pSin_parts_ptr[pEm->Neck_no = 3];
    pEm->Neck_flg = 0;
    PW_FREE(pP)[3] = PW_FREE(pP)[2] = 0x40;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[5] = 0x138;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = Em22_life_easy[(Rnd_l() >> (Rnd_l() & 3)) & 0xF];
    else
        pEm->Max_life = pEm->Life = Em22_life[(Rnd_l() >> (Rnd_l() & 3)) & 0xF];
    EM22_W(pEm)->Unk26 = 150;
    EM22_W(pEm)->Unk12 = 0;
    EM22_W(pEm)->Unk02 = 0;
    EM22_W(pEm)->Unk08 = 0;
    EM22_W(pEm)->Unk14 = 0;
    EM22_W(pEm)->Unk1A = 0;
    EM22_W(pEm)->Unk1B = 0;
    EM22_W(pEm)->Unk18 = 0;
    EM22_W(pEm)->Unk23 = 0;
    EM22_W(pEm)->Unk16 = 0;
    EM22_W(pEm)->Unk0E = 0;
    EM22_W(pEm)->Unk0F = 0;
    EM22_W(pEm)->Unk19 = 0;
    EM22_W(pEm)->Unk2B = 0;
    EM22_W(pEm)->Unk11 = 0;
    EM22_W(pEm)->Unk10 = 0;
    pEm->Max_life += 10;
    pEm->Life += 10;
    EM22_W(pEm)->Unk03 = (Rnd_l() & 0x1F) + 30;
    EM22_W(pEm)->Unk13 = 1;
    pEm->pSin_parts_ptr[1].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[5].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[7].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[8].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[10].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[11].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[12].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[13].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[14].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[15].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[16].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[17].Be_flg |= 0x20;
    if (pEm->Id == 0x22) {
        G.pPldie_Em[pEm->Id - 0x10] = func_80108448;
        pP = &pEm->pSin_parts_ptr[20];
        pP->UnkA2 = 0;
    } else {
        G.pPldamage_Em[pEm->Id - 0x10] = func_80108680;
        G.pPldie_Em[pEm->Id - 0x10] = func_8010880C;
    }
    if (pEm->Type == 0x4001) {
        EM_SET_RTN(pEm, 4, 0, 0, 0);
        pEm->Be_flg |= 0x2030;
        pEm->Damage_cnt |= 0x80;
    }
    if (pEm->Type == 0x5002) {
        EM_SET_RTN(pEm, 4, 1, 0, 0);
        pEm->Be_flg |= 0x2030;
        pEm->Damage_cnt |= 0x80;
    }
    if (pEm->Type == 0x5003) {
        pEm->Pos_x = -12945;
        pEm->Pos_z = -11485;
        pEm->Pos_y = -3600;
        EM_SET_RTN(pEm, 4, 3, 0, 0);
        pEm->Be_flg |= 0x2030;
        pEm->Damage_cnt |= 0x80;
    }
    if ((pEm->Type & 0xF) == 4)
        EM22_W(pEm)->Unk13 = 0;
}

/* RE2 Em_move */
void func_80100680(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    tagGLOBAL *pG;
    SCA_HEAD *pSca;

    if (Guide_check_(pEm->Pos_x, pEm->Pos_z, pEm->nFloor) != 0xFF) {
        Root_ck(pEm, POS_V(pPl), 0, 0);
    } else {
        pEm->Dest_x = pPl->Pos_x;
        pEm->Dest_z = pPl->Pos_z;
    }
    pG = &G;
    pSca = pG->pRoom->pSca;
    Sca_search(func_8004D6B8_(pEm->Pos_x, pEm->Pos_z, pSca->Num, pSca->pad2), 0x100, pEm->nFloor);
    EM22_W(pEm)->Unk28 = func_80108CA0();
    Em22_think_tbl[pEm->Routine_1](pEm);
    Em22_move_tbl[pEm->Routine_1](pEm, pKan, pSeq);
    EM22_W(pEm)->Old_spd_x = pEm->Spd_x;
    Hit_area_parts_set(pEm, 0xFCE00001, 0x1F40010);
    if (EM22_W(pEm)->Unk1B)
        EM22_W(pEm)->Unk1B--;
    if (EM22_W(pEm)->Unk26)
        EM22_W(pEm)->Unk26--;
    pEm->pEnemy_ptr = pEm->pUnk114 = PLAYER_EM(pG);
    Rot_neck_em(pEm, pEm->Cdir_y);
    if (pEm->Id == 0x24) {
        if (*pEm->pNow_seq & 0x800)
            pEm->Unk16A = 1;
        func_80108DAC(pEm);
    }
}

/* think routine 0 */
void func_80100844(ENEMY_WORK *pEm)
{
    LONG Side = 1;
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist;
    SHORT Dir;
    ULONG Flg;

    Dist = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Flg = (USHORT)(pEm->Status_flg & 0x2000);
    if (EM22_W(pEm)->Unk03) {
        EM22_W(pEm)->Unk03--;
        if (Dist < 3000)
            EM22_W(pEm)->Unk03 = 0;
    } else {
        if (!EM22_W(pEm)->Unk02) {
            if (Flg && !Dir) {
                EM22_W(pEm)->Unk02 = Side;
                EM22_W(pEm)->Unk1B = 30;
            } else if (Flg && EM22_W(pEm)->Unk16 != 0xFF) {
                EM22_W(pEm)->Unk19 = 0;
                EM_SET_RTN(pEm, 1, 5, 0, 0);
            } else {
                EM_SET_RTN(pEm, 1, 1, 0, 0);
            }
            if (!EM22_W(pEm)->Unk02)
                return;
        }
        if (EM22_W(pEm)->Unk16 != 0xFF) {
            if (Dist > 6400 || !EM22_W(pEm)->Unk13) {
                EM22_W(pEm)->Unk19 = 0;
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            } else if (func_80103260(pEm)) {
                EM22_W(pEm)->Unk10 = 0;
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            } else {
                EM_SET_RTN(pEm, 1, 1, 0, 0);
                if (Flg) {
                    Side = func_80101A30(pEm);
                    if (EM22_W(pEm)->Unk28 && EM22_W(pEm)->Unk13) {
                        if (Dist < 5000 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100) && func_80108CB8(pEm, 0x401)) {
                            if (Side)
                                EM_SET_RTN(pEm, 1, 4, 0, 0);
                            else
                                EM_SET_RTN(pEm, 1, 3, 0, 0);
                        }
                        if (Dist < 3000 && !Dir) {
                            if (func_80101988(pEm)) {
                                if (Side)
                                    EM_SET_RTN(pEm, 1, 4, 0, 0);
                            } else {
                                EM_SET_RTN(pEm, 1, 3, 0, 0);
                            }
                        }
                    }
                }
            }
        } else {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
    }
}

/* move routine 0 */
void func_80100A54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0xF);
        EM22_W(pEm)->Unk02 = 0;
        pEm->Routine_2 = 1;
        Spd_get2(pEm, pKan, pSeq, 0);
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
    }
    Add_speedXZ(pEm, 0);
}

/* think routine 1 */
void func_80100B08(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist;
    LONG Bite;
    LONG Side;
    SHORT Dir1;
    SHORT Dir2;
    ULONG Flg;
    LONG Ret;

    Dist = pEm->Unk154;
    Bite = func_80103260(pEm);
    Side = func_80101A30(pEm);
    if (pEm->UnkDA)
        pEm->UnkDA--;
    Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 600);
    Dir2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 350);
    if (!EM22_W(pEm)->Unk13) {
        Flg = (LONG)pEm->Unk154 < 7000;
        if (Flg) {
            EM22_W(pEm)->Unk02 = 1;
            EM22_W(pEm)->Unk1B = 5;
            EM22_W(pEm)->Unk19 = 0;
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
        return;
    }
    Flg = (USHORT)(pEm->Status_flg & 0x2000);
    if ((LONG)pEm->Unk154 < 2500)
        Flg = 1;
    if (!EM22_W(pEm)->Unk02) {
        if (Flg && !Dir2) {
            EM22_W(pEm)->Unk02 = 1;
            if (!EM22_W(pEm)->Unk1B)
                EM22_W(pEm)->Unk1B = 15;
        }
    }
    if (EM22_W(pEm)->Unk02) {
        if (EM22_W(pEm)->Unk16 != 0xFF) {
            if (Dist > 6400) {
                EM22_W(pEm)->Unk19 = 0;
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            }
            if (!pEm->UnkDA && EM22_W(pEm)->Unk28 && (pEm->Status_flg & 0x2000)) {
                if (Dist < 5000 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100) && func_80108CB8(pEm, 0x401)
                    && EM22_W(pEm)->Unk13) {
                    if (Side)
                        EM_SET_RTN(pEm, 1, 4, 0, 0);
                    else
                        EM_SET_RTN(pEm, 1, 3, 0, 0);
                }
                if (Dist < 3000 && !Dir2) {
                    if (!func_80101988(pEm))
                        EM_SET_RTN(pEm, 1, 3, 0, 0);
                    else if (EM22_W(pEm)->Unk13 && Side)
                        EM_SET_RTN(pEm, 1, 4, 0, 0);
                }
                if (pPl->Life < 71) {
                    if (pEm->Id == 0x22) {
                        if (Dist < 6400 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80) && func_80108D44()
                            && func_80108CB8(pEm, 0x601))
                            EM_SET_RTN(pEm, 1, 6, 0, 0);
                    } else if (Dist < 5400 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0xA4) && func_80108D44()) {
                        EM_SET_RTN(pEm, 1, 0xD, 0, 0);
                    }
                }
            }
            if (!(Rnd_l() & 0xF) && !Dir2 && Bite)
                EM_SET_RTN(pEm, 1, 7, 0, 0);
        } else if (!pEm->UnkDA && EM22_W(pEm)->Unk28 && (pEm->Status_flg & 0x2000)) {
            if (Dist < 3000 && !Dir2) {
                if (!func_80101988(pEm))
                    EM_SET_RTN(pEm, 1, 3, 0, 0);
                else if (EM22_W(pEm)->Unk13 && Side)
                    EM_SET_RTN(pEm, 1, 4, 0, 0);
            }
            if (pPl->Life < 71) {
                if (pEm->Id == 0x22) {
                    if (Dist < 6400 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80) && func_80108D44()
                        && func_80108CB8(pEm, 0x601))
                        EM_SET_RTN(pEm, 1, 6, 0, 0);
                } else if (Dist < 5400 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0xA4) && func_80108D44()) {
                    EM_SET_RTN(pEm, 1, 0xD, 0, 0);
                }
            }
        }
    } else if (EM22_W(pEm)->Unk16 != 0xFF && Dist < 3000) {
        pEm->Move_no = 2;
        EM22_W(pEm)->Unk19 = 0;
        pEm->Hokan_flg = 15;
        EM_SET_RTN(pEm, 1, 5, 1, 0);
    }
    if (EM22_W(pEm)->Unk16 != 0xFF) {
        if (Dist < 2000 && !(Rnd_l() & 7)) {
            if (Rnd_l() & 1) {
                if (!EM22_W(pEm)->Unk26 && Dir1)
                    EM_SET_RTN(pEm, 1, 8, 0, 0);
            } else if (Bite) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            }
        } else if (!EM22_W(pEm)->Unk26 && Dir1) {
            if (Rnd_l() & 1)
                EM_SET_RTN(pEm, 1, 8, 0, 0);
            else
                EM22_W(pEm)->Unk19 = (Rnd_l() & 0x1F) + 30;
        }
        if (Dist - 1801 < 4199 && pPl->Routine_1 == 5 && pPl->Routine_2 && pPl->pEnemy_ptr == pEm) {
            Ret = func_80104FF0(pEm);
            if (Ret) {
                EM_SET_RTN(pEm, 1, 0xE, 0, 0);
                pEm->Routine_3 = Ret;
            }
        }
    }
    if (!EM22_W(pEm)->Unk1B && (LONG)pEm->Unk154 < 3000 && pPl->Routine_0 == 5
        && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300) && func_80108CB8(pEm, 0xF01))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    if (pPl->Routine_1 == 5 && pPl->Routine_2 && pPl->pEnemy_ptr == pEm && (Rnd_l() & 3) == 1
        && (pEm->Status_flg & 0x2000) && Side && Dist < 5000 && !Dir1 && func_80108CB8(pEm, 0x401)
        && EM22_W(pEm)->Unk13)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist < 2000)
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (Rnd_l() + 0x18) & 0xF);
    else
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (Rnd_l() + 0x10) & 0xF);
}

/* move routine 1 */
void func_801011BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        if (EM22_W(pEm)->Unk16 != 0xFF)
            EM_SET_MOVE(pEm, 2, 0, 0xF);
        else
            EM_SET_MOVE(pEm, 0x12, 0, 0xF);
    case 1:
        pEm->Timer0 = (Rnd_l() & 0x1F) + 300;
        pEm->Routine_2 = 2;
        Spd_get2(pEm, pKan, pSeq, 0);
    case 2:
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        if (--pEm->Timer0 == 0) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            EM22_W(pEm)->Unk03 = Rnd_l() & 0x3F;
        }
    }
    Add_speedXZ(pEm, 0);
    func_80109058(pEm);
}

/* think routine 2 */
void func_801012E4(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist;
    LONG Dir1;
    LONG Dir2;
    ULONG Flg;
    LONG Side;
    LONG Ret;
    LONG Spd;

    Dist = pEm->Unk154;
    Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x40);
    Dir2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x180);
    Flg = (USHORT)(pEm->Status_flg & 0x2000);
    Side = func_80101A30(pEm);
    if (++EM22_W(pEm)->Unk19 == 90)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (EM22_W(pEm)->Unk23)
        EM22_W(pEm)->Unk23--;
    if (Flg && !EM22_W(pEm)->Unk23 && Dist < 3000) {
        if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80) && EM22_W(pEm)->Unk28) {
            if (!func_80101988(pEm))
                EM_SET_RTN(pEm, 1, 3, 0, 0);
            else if (EM22_W(pEm)->Unk13 && Side)
                EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
    }
    if (Flg && !EM22_W(pEm)->Unk23 && EM22_W(pEm)->Unk02 && EM22_W(pEm)->Unk28) {
        if (Dist < 3000 && !Dir2) {
            if (!func_80101988(pEm))
                EM_SET_RTN(pEm, 1, 3, 0, 0);
            else if (EM22_W(pEm)->Unk13 && Side)
                EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        if (Dist < 5000 && !Dir1 && func_80108CB8(pEm, 0x401) && EM22_W(pEm)->Unk13) {
            if (Side)
                EM_SET_RTN(pEm, 1, 4, 0, 0);
            else
                EM_SET_RTN(pEm, 1, 3, 0, 0);
        }
        if (!EM22_W(pEm)->Unk1B && pPl->Life < 71) {
            if (pEm->Id == 0x22) {
                if (Dist < 6400 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80) && func_80108D44()
                    && func_80108CB8(pEm, 0x601))
                    EM_SET_RTN(pEm, 1, 6, 0, 0);
            } else if (Dist < 5400 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0xA4) && func_80108D44()) {
                EM_SET_RTN(pEm, 1, 0xD, 0, 0);
            }
        }
    }
    if (pPl->Routine_1 == 5 && pPl->Routine_2 && pPl->pEnemy_ptr == pEm) {
        if ((Rnd_l() & 3) == 1) {
            if (Dist - 3001 < 1999 && !Dir1) {
                Ret = func_80104FF0(pEm);
                if (Ret) {
                    EM_SET_RTN(pEm, 1, 0xE, 0, 0);
                    pEm->Routine_3 = Ret;
                }
            }
        } else if (Rnd_l() & 1) {
            if (Flg && Dist < 5000 && !Dir1 && func_80108CB8(pEm, 0x401) && EM22_W(pEm)->Unk13) {
                if (Side)
                    EM_SET_RTN(pEm, 1, 4, 0, 0);
                else
                    EM_SET_RTN(pEm, 1, 3, 0, 0);
            }
        }
    }
    if (Dist < 3000) {
        Spd = (LONG)(3000 - Dist) / 20;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (Rnd_l() & 0x1F) + 108 + Spd);
    } else
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);
    if (pEm->Col.Hit_flg & 0x20) {
        if (++EM22_W(pEm)->Unk0E >= 11 && Side && !EM22_W(pEm)->Unk13 && pEm->nFloor == pPl->nFloor
            && pEm->Pos_y == pPl->Pos_y && func_80108CB8(pEm, 0x401))
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        if (EM22_W(pEm)->Unk0E >= 61) {
            EM22_W(pEm)->Unk0E = 0;
            EM22_W(pEm)->Unk19 = 0;
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        }
    }
    if (pEm->Col.Unk3A & 0x3FFF) {
        EM22_W(pEm)->Unk0E++;
        if (EM22_W(pEm)->Unk0E > (Rnd_l() & 0xF) + 20) {
            if ((Rnd_l() & 3) == 1) {
                EM22_W(pEm)->Unk0E = 0;
                Ret = func_80104FF0(pEm);
                if (Ret) {
                    EM_SET_RTN(pEm, 1, 0xE, 0, 0);
                    pEm->Routine_3 = Ret;
                } else if (Rnd_l() & 1) {
                    EM_SET_RTN(pEm, 1, 8, 0, 0);
                }
            } else if ((pEm->Status_flg & 0x2000) && Side && func_80108CB8(pEm, 0x401) && EM22_W(pEm)->Unk13) {
                EM_SET_RTN(pEm, 1, 4, 0, 0);
            }
        }
    }
}

/* move routine 2 */
void func_80101858(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 0xF);
        pEm->Routine_2 = 1;
        Spd_get2(pEm, pKan, pSeq, 0);
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
    }
    Add_speedXZ(pEm, 0);
    func_80109058(pEm);
}

/* point 800 ahead and to the side of pEm, relative to pT, into Unk0A/Unk0C */
void func_80101914(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SVECTOR Sv;

    Sv.vx = 800;
    Sv.vz = 0;
    Rot_vector_l(pEm->Cdir_y + 0x400, &Sv, &Sv);
    EM22_W(pEm)->Unk0A = pT->Pos_x + Sv.vx;
    EM22_W(pEm)->Unk0C = pT->Pos_z + Sv.vz;
}

LONG func_80101988(ENEMY_WORK *pEm)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[11];
    LONG Ceil;

    if ((pEm->Type & 0x100) && Flg_ck(&G.Unk7888, 0x1F))
        return 1;
    if (__builtin_abs(pEm->nFloor - Pl_work.nFloor) >= 2)
        return 1;
    Ceil = pEm->Unk122 - 1600;
    return Sca_get_ground(pP->Workm.t, pEm->Col.W0, 0x200, 0) < Ceil;
}

LONG func_80101A30(ENEMY_WORK *pEm)
{
    tagGLOBAL *pG = &G;

    if (__builtin_abs(pEm->nFloor - pG->Player.nFloor) >= 2)
        return 0;
    if ((pEm->Type & 0x100) && Flg_ck(&pG->Unk7888, 0x1F))
        return 0;
    return 1;
}

/* think routine 3 */
void func_80101AA4(ENEMY_WORK *pEm)
{
    pEm->Unk16A = 0;
}

/* move routine 3: claw attack */
void func_80101AAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    SHORT Box0[4];
    SHORT Box1[4];
    LONG Unused[4];
    HIT_AREA Area0;
    HIT_AREA Area1;
    LONG *pPos;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 3);
        pEm->Routine_2 = 1;
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->UnkE2 = 0;
        EM22_W(pEm)->Unk17 = 0;
        func_8002646C_(pEm, 400, 40, pEm->Col.Unk39);
    case 1:
        if (EM22_W(pEm)->Unk17 == 4)
            Snd_se_enem(9, pEm);
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            pEm->pSin_parts_ptr[11].Poly_rgb = 0x808080;
            pEm->pSin_parts_ptr[12].Poly_rgb = 0x808080;
            pEm->pSin_parts_ptr[13].Poly_rgb = 0x808080;
            pEm->UnkDA = (Rnd_l() & 0xF) + 10;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Be_flg |= 0x80000;
            break;
        }
        if (++EM22_W(pEm)->Unk17 < 15 && __builtin_abs(Pl_work.nFloor - pEm->nFloor) < 2) {
            Box0[0] = 700;
            Box0[1] = 0;
            Box0[2] = 600;
            Box0[3] = 1400;
            Area0.pPos = WORKM_POS_V(&pEm->pSin_parts_ptr[8]);
            Area0.pBox = Box0;
            Area0.Dir = pEm->Cdir_y;
            if (!pEm->Water) {
                Area0.Id = 0x85;
                if ((Rnd_l() & 3) == 1)
                    Area0.Id = 0x82;
            } else {
                Area0.Id = 0x82;
            }
            pPos = &pEm->Pos_x;
            func_800445C8(pPos, pPos, &Area0, pPl);
            Box1[0] = 2000;
            Box1[1] = 0;
            Box1[2] = 700;
            Box1[3] = 1400;
            Area1.pPos = WORKM_POS_V(&pEm->pSin_parts_ptr[8]);
            Area1.pBox = Box1;
            Area1.Dir = pEm->Cdir_y;
            Area1.Id = 0x80;
            func_800445C8(pPos, pPos, &Area1, pPl);
        }
        if ((ULONG)(EM22_W(pEm)->Unk17 - 7) < 4) {
            Sv.vx = 400;
            Sv.vz = 0;
            Sv.vy = 0;
            Sv.pad = 800;
            if (!pPl->Damage_cnt && func_80044420(&pEm->pSin_parts_ptr[9].Workm, &Sv, pPl) && !pEm->UnkE2) {
                Snd_se_enem(5, pEm);
                pEm->pSin_parts_ptr[11].Poly_rgb = 0x202070;
                pEm->pSin_parts_ptr[12].Poly_rgb = 0x303075;
                pEm->pSin_parts_ptr[13].Poly_rgb = 0x282875;
                Pad_vib_set0(7, 0);
                Pad_vib_set1(3, 250, 4);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                Sv.vx = Rnd_l() & 0x3F;
                Sv.vy = 150;
                Sv.vz = 0;
                Esp_call(0, (USHORT)pPl->Cdir_y | 0x14000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, 0);
                if (Pl_life_down(15, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
                pEm->UnkE2 = 1;
            }
        }
        if (pEm->UnkE2 && pEm->UnkE2 < 6) {
            if (pEm->UnkE2 < 4) {
                Sv.vx = (4 - pEm->UnkE2) * 50;
                Sv.vz = 0;
                Add_speedXZ(pPl, pPl->Cdir_y - pEm->Cdir_y);
            }
            pEm->UnkE2++;
            Sv.vx = 400;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, 0);
        }
    }
    Add_speedXZ(pEm, 0);
}

/* point 200 ahead of pEm, relative to pT, into Unk0A/Unk0C */
void func_80101EF0(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SVECTOR Sv;

    Sv.vx = 200;
    Sv.vz = 0;
    Rot_vector(pEm->Cdir_y, &Sv, &Sv);
    EM22_W(pEm)->Unk0A = pT->Pos_x + Sv.vx;
    EM22_W(pEm)->Unk0C = pT->Pos_z + Sv.vz;
    EM22_W(pEm)->Unk2A = pEm->Unk154;
}

/* think routine 4 */
void func_80101F60(ENEMY_WORK *pEm)
{
    if ((ULONG)(pEm->Move_cnt - 12) < 10)
        pEm->Unk132 |= 0x4000;
    else
        pEm->Unk132 &= 0xBFFF;
    if (pEm->Move_cnt == 8 && EM22_W(pEm)->Unk13) {
        if (!(Rnd_l() & 0xF) || !func_80101A30(pEm)) {
            EM22_W(pEm)->Unk23 = 15;
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
        if ((LONG)pEm->Unk154 < 2800 && (Rnd_l() & 1) && !func_80101988(pEm))
            EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    pEm->Unk16A = 0;
}

/* move routine 4: jump attack */
void func_80102044(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    HIT_AREA Area;
    LONG Unused[2];
    VECTOR V;
    LONG Dir;

    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x180);
    func_80101EF0(pEm, pPl);
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Routine_2 = 1;
        Spd_get2(pEm, pKan, pSeq, 0);
        EM22_W(pEm)->Unk08 = 1;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        EM22_W(pEm)->Unk17 = 0;
        EM22_W(pEm)->Unk1E = 0;
        EM22_W(pEm)->Unk00 = pEm->Unk122;
        pEm->Unk152 = pEm->Unk154;
        if (!EM22_W(pEm)->Unk13 && pEm->nFloor == pPl->nFloor)
            pEm->Be_flg |= 0xA000;
    case 1:
        if (pEm->Move_cnt == 9) {
            if (((pEm->Type & 0xF) == 4 && !EM22_W(pEm)->Unk13)
                || (G.Unk783E == 4 && G.Unk7840 == 7 && pEm->Water)) {
                V.vx = pEm->Pos_x;
                V.vy = pEm->Pos_y;
                V.vz = pEm->Pos_z;
                Snd_se_stad(0x10201, &V, 0, 0);
            } else {
                Snd_se_enem(2, pEm);
            }
        }
        if (EM22_W(pEm)->Unk17 == 18)
            Snd_se_enem(10, pEm);
        Spd_get2(pEm, pKan, pSeq, 0);
        if (pEm->Unk152 < (LONG)pEm->Unk154) {
            if (EM22_W(pEm)->Unk1E < 280)
                EM22_W(pEm)->Unk1E += 40;
        } else if (EM22_W(pEm)->Unk1E > 0) {
            EM22_W(pEm)->Unk1E -= 40;
        }
        pEm->Unk152 = pEm->Unk154;
        if ((ULONG)(pEm->Move_cnt - 9) < 11 && EM22_W(pEm)->Unk13)
            pEm->Spd_x += EM22_W(pEm)->Unk1E;
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            if (!EM22_W(pEm)->Unk13) {
                EM22_W(pEm)->Unk13 = 2;
                pEm->Be_flg &= 0xFFFF5FFF;
            }
            pEm->pSin_parts_ptr[11].Poly_rgb = 0x808080;
            pEm->pSin_parts_ptr[12].Poly_rgb = 0x808080;
            pEm->pSin_parts_ptr[13].Poly_rgb = 0x808080;
            EM22_W(pEm)->Unk08 = 0;
            if ((LONG)pEm->Unk154 < 3000 && !Dir && (Rnd_l() & 1) && (pEm->Status_flg & 0x2000)) {
                if (func_80101988(pEm)) {
                    if (func_80101A30(pEm))
                        EM_SET_RTN(pEm, 1, 4, 0, 0);
                } else {
                    EM_SET_RTN(pEm, 1, 3, 0, 0);
                }
            } else {
                pEm->UnkDA = (Rnd_l() & 0x3F) + 50;
                if (Rnd_l() & 1) {
                    EM_SET_RTN(pEm, 1, 1, 0, 0);
                    EM22_W(pEm)->Unk03 = Rnd_l() & 0x3F;
                } else {
                    EM_SET_RTN(pEm, 1, 2, 0, 0);
                    EM22_W(pEm)->Unk1B = 10;
                    EM22_W(pEm)->Unk19 = 0;
                    EM22_W(pEm)->Unk02 = 1;
                }
            }
            break;
        }
        if (pEm->Move_cnt >= 8 && !EM22_W(pEm)->Unk13 && func_801091A0(pEm)) {
            pEm->Col.W0 = pEm->Col.D0 = 200;
            pEm->Be_flg &= 0xFFFF5FFF;
        }
        if (++EM22_W(pEm)->Unk17 < 16 && __builtin_abs(Pl_work.nFloor - pEm->nFloor) < 2) {
            Sv.vx = 1400;
            Sv.vy = 200;
            Sv.vz = 1200;
            Sv.pad = 1200;
            Area.pPos = WORKM_POS_V(&pEm->pSin_parts_ptr[8]);
            Area.pBox = &Sv.vx;
            Area.Dir = pEm->Cdir_y;
            if (!pEm->Water) {
                if (Rnd_l() & 1)
                    Area.Id = 0xA2;
                else
                    Area.Id = 0x81;
            } else if (Rnd_l() & 1) {
                Area.Id = 0x82;
            } else {
                Area.Id = 0x81;
            }
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        }
        if ((ULONG)(pEm->Move_cnt - 15) < 10) {
            Sv.vx = 400;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 800;
            if (!pPl->Damage_cnt && func_80044420(&pEm->pSin_parts_ptr[9].Workm, &Sv, pPl) && !pEm->UnkDA) {
                pEm->pSin_parts_ptr[11].Poly_rgb = 0x202070;
                pEm->pSin_parts_ptr[12].Poly_rgb = 0x303075;
                pEm->pSin_parts_ptr[13].Poly_rgb = 0x282875;
                Pad_vib_set0(7, 0);
                Pad_vib_set1(3, 250, 4);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl) + 2;
                pPl->Routine_2 = 0;
                Snd_se_enem(3, pEm);
                Sv.vx = 0;
                Sv.vz = 0;
                Sv.vy = 0;
                Esp_call(0, (USHORT)pPl->Cdir_y | 0x14000000, &pPl->pSin_parts_ptr->Workm, &Sv, 0);
                if (Pl_life_down(25, 0) & 2)
                    EM_SET_RTN(pPl, 3, 0, 0, 0);
                pEm->UnkDA = 1;
            }
        }
        if (pEm->UnkDA && pEm->UnkDA < 6) {
            if (pEm->UnkDA < 5) {
                pPl->Spd_x = (5 - pEm->UnkDA) * 50;
                pPl->Spd_z = 0;
                Add_speedXZ(pPl, pPl->Cdir_y - pEm->Cdir_y);
            }
            pEm->UnkDA++;
            Sv.vx = 0;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, 0);
        }
        if (EM22_W(pEm)->Unk17 == 27)
            Snd_se_enem(4, pEm);
    }
    if (pEm->Move_cnt >= 16 && pEm->Water) {
        if (pEm->Water < pEm->pSin_parts_ptr[0].Workm.t[1] && pEm->UnkE2 < 2) {
            pEm->UnkE2++;
            func_80108AAC(pEm);
        } else if (pEm->Water < pEm->pSin_parts_ptr[16].Workm.t[1] && pEm->UnkE2 < 2) {
            pEm->UnkE2++;
            func_80108AAC(pEm);
        }
    }
    if (pEm->Move_cnt >= 10 && (pEm->Type & 0xF) == 4 && !EM22_W(pEm)->Unk13 && pEm->UnkE2 < 2) {
        pEm->UnkE2++;
        func_80108AAC(pEm);
    }
    if (EM_RTN01(pPl) != 0x701 && EM22_W(pEm)->Unk13)
        Goto00(pEm, EM22_W(pEm)->Unk0A, EM22_W(pEm)->Unk0C, 0x18);
    Add_speedXZ(pEm, 0);
}

/* think routine 5 */
void func_8010281C(ENEMY_WORK *pEm)
{
    if (EM22_W(pEm)->Unk19 < 32)
        EM22_W(pEm)->Unk19++;
}

/* move routine 5 */
void func_8010283C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Spd;
    LONG End;
    LONG Next;

    switch (pEm->Routine_2) {
    case 0:
        if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) && !EM22_W(pEm)->Unk26) {
            EM22_W(pEm)->Unk19 = 80;
            EM_SET_MOVE(pEm, 0x1D, 0, 0xF);
            EM22_W(pEm)->Unk26 = (Rnd_l() & 0x1F) + 120;
        } else {
            EM22_W(pEm)->Unk19 = 0;
            EM_SET_MOVE(pEm, 2, 0, 0xF);
        }
        pEm->Routine_2 = 1;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        End = Joint_move(pEm, pKan, pSeq, 0x100);
        Spd = EM22_W(pEm)->Unk19;
        if (Spd < 33) {
            if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80)) {
                EM22_W(pEm)->Unk02 = 1;
                if (!EM22_W(pEm)->Unk1B)
                    EM22_W(pEm)->Unk1B = 50;
                pEm->UnkDA = 0;
                EM22_W(pEm)->Unk19 = 0;
                EM_SET_RTN(pEm, 1, 1, 1, 0);
            }
        } else {
            if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80))
                Next = Spd + 2;
            else
                Next = Spd;
            EM22_W(pEm)->Unk19 = Next;
            if (End) {
                EM22_W(pEm)->Unk02 = 1;
                if (!EM22_W(pEm)->Unk1B)
                    EM22_W(pEm)->Unk1B = 50;
                pEm->UnkDA = 0;
                EM22_W(pEm)->Unk19 = 0;
                EM_SET_RTN(pEm, 1, 1, 1, 0);
            }
            pEm->Spd_x >>= 2;
            Add_speedXZ(pEm, 0);
        }
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, EM22_W(pEm)->Unk19);
    }
}

/* think routine 6 */
void func_80102A10(ENEMY_WORK *pEm)
{
    LONG Lim;

    if ((ULONG)(pEm->Move_cnt - 10) < 14) {
        if (pEm->Move_cnt == 10) {
            EM22_W(pEm)->Unk2A = pEm->Unk154;
            EM22_W(pEm)->Unk1E = -250;
            EM22_W(pEm)->Unk2A /= 3;
        } else {
            EM22_W(pEm)->Unk1E += 40;
        }
        if (EM22_W(pEm)->Unk2A < (LONG)pEm->Unk154 && EM22_W(pEm)->Unk1E < 0)
            EM22_W(pEm)->Unk1E = 0;
    }
    pEm->Pos_y += EM22_W(pEm)->Unk1E;
    Lim = -(pEm->nFloor * 1800);
    if (Lim < pEm->Pos_y) {
        pEm->Pos_y = Lim;
        EM22_W(pEm)->Unk1E = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/enemy/em22", func_80102AEC);

INCLUDE_ASM("asm/nonmatchings/enemy/em22", func_80103110);

/* two way points 1800 / 3600 to one side of pEm (Unk2C/Unk2E, Unk0A/Unk0C) */
void func_8010316C(ENEMY_WORK *pEm, LONG Flg, VECTOR *pV)
{
    if (Flg) {
        if (Rnd_l() & 1) {
            EM22_W(pEm)->Unk2C = pV->vx;
            EM22_W(pEm)->Unk2E = pEm->Pos_z - 1800;
            EM22_W(pEm)->Unk0A = pEm->Pos_x;
            EM22_W(pEm)->Unk0C = pEm->Pos_z - 3600;
        } else {
            EM22_W(pEm)->Unk2C = pV->vx;
            EM22_W(pEm)->Unk2E = pEm->Pos_z + 1800;
            EM22_W(pEm)->Unk0A = pEm->Pos_x;
            EM22_W(pEm)->Unk0C = pEm->Pos_z + 3600;
        }
    } else if (Rnd_l() & 1) {
        EM22_W(pEm)->Unk2C = pEm->Pos_x - 1800;
        EM22_W(pEm)->Unk2E = pV->vz;
        EM22_W(pEm)->Unk0C = pEm->Pos_z;
        EM22_W(pEm)->Unk0A = pEm->Pos_x - 3600;
    } else {
        EM22_W(pEm)->Unk2C = pEm->Pos_x + 1800;
        EM22_W(pEm)->Unk2E = pV->vz;
        EM22_W(pEm)->Unk0C = pEm->Pos_z;
        EM22_W(pEm)->Unk0A = pEm->Pos_x + 3600;
    }
}

/* look for a wall edge to climb along (searches the shapes found by Sca_search) */
LONG func_80103260(ENEMY_WORK *pEm)
{
    VECTOR V;
    LONG Ret;
    LONG i;

    if (pEm->Id == 0x24)
        return 0;
    i = 0;
    Ret = 1;
    EM22_W(pEm)->Unk29 = Rnd_l() & 1;
    switch (func_80103110(pEm, EM22_W(pEm)->Unk29)) {
    case 0:
        for (i = 0; i < 30 && Ret; i++) {
            if (!G.Sca_list[i]) {
                i = 30;
                break;
            }
            if (G.Sca_list[i]->X0 + 3500 < pEm->Pos_x && pEm->Pos_x < G.Sca_list[i]->X1 - 3500) {
                V.vx = pEm->Pos_x - 2700;
                V.vz = G.Sca_list[i]->Z1 + 300;
                Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                if (__builtin_abs(pEm->Pos_z - V.vz) < 1200 && !Ret)
                    func_8010316C(pEm, 0, &V);
                else
                    Ret = 1;
            }
        }
        break;
    case 1:
        for (; i < 30 && Ret; i++) {
            if (!G.Sca_list[i]) {
                i = 30;
                break;
            }
            if (G.Sca_list[i]->Z0 + 3500 < pEm->Pos_z && pEm->Pos_z < G.Sca_list[i]->Z1 - 3500) {
                V.vx = G.Sca_list[i]->X1 + 300;
                V.vz = pEm->Pos_z - 2700;
                Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                if (__builtin_abs(pEm->Pos_x - V.vx) < 1200 && !Ret)
                    func_8010316C(pEm, 1, &V);
                else
                    Ret = 1;
            }
        }
        break;
    case 2:
        for (; i < 30 && Ret; i++) {
            if (!G.Sca_list[i]) {
                i = 30;
                break;
            }
            if (G.Sca_list[i]->X0 + 3500 < pEm->Pos_x && pEm->Pos_x < G.Sca_list[i]->X1 - 3500) {
                V.vx = pEm->Pos_x - 2700;
                V.vz = G.Sca_list[i]->Z0 - 300;
                Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                if (__builtin_abs(pEm->Pos_z - V.vz) < 1200 && !Ret)
                    func_8010316C(pEm, 0, &V);
                else
                    Ret = 1;
            }
        }
        break;
    case 3:
        for (; i < 30 && Ret; i++) {
            if (!G.Sca_list[i]) {
                i = 30;
                break;
            }
            if (G.Sca_list[i]->Z0 + 3500 < pEm->Pos_z && pEm->Pos_z < G.Sca_list[i]->Z1 - 3500) {
                V.vx = G.Sca_list[i]->X0 - 300;
                V.vz = pEm->Pos_z - 2700;
                Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                if (__builtin_abs(pEm->Pos_x - V.vx) < 1200 && !Ret)
                    func_8010316C(pEm, 1, &V);
                else
                    Ret = 1;
            }
        }
        break;
    }
    return i != 30;
}

/* think routine 7 */
void func_801036B0(ENEMY_WORK *pEm)
{
}

/* move routine 7: jump to a wall (way points Unk2C/Unk2E, Unk0A/Unk0C) */
void func_801036B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Unused[4];

    switch (pEm->Routine_2) {
    case 0:
        if (EM22_W(pEm)->Unk29)
            EM_SET_MOVE(pEm, 0x15, 0, 3);
        else
            EM_SET_MOVE(pEm, 0x13, 0, 3);
        pEm->Routine_2++;
        EM22_W(pEm)->Unk04 = pEm->Pos_x;
        EM22_W(pEm)->Unk06 = pEm->Pos_z;
        pEm->Be_flg |= 0x8000;
    case 1:
        if (pEm->Move_cnt >= 5) {
            if (pEm->Move_cnt < 11) {
                EM22_W(pEm)->Unk08 = 1;
                pEm->Pos_x = EM22_W(pEm)->Unk04
                    + (EM22_W(pEm)->Unk2C - EM22_W(pEm)->Unk04) * Em22_jump_tbl[pEm->Move_cnt - 5] / 25;
                pEm->Pos_z = EM22_W(pEm)->Unk06
                    + (EM22_W(pEm)->Unk2E - EM22_W(pEm)->Unk06) * Em22_jump_tbl[pEm->Move_cnt - 5] / 25;
            } else {
                pEm->Cdir_y = EM22_W(pEm)->Unk00;
            }
        }
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            if (Rnd_l() & 3)
                EM_SET_RTN(pEm, 1, 9, 0, 0);
            else
                pEm->Routine_2++;
        }
        break;
    case 2:
        if (EM22_W(pEm)->Unk29)
            EM_SET_MOVE(pEm, 0x16, 0, 5);
        else
            EM_SET_MOVE(pEm, 0x14, 0, 5);
        pEm->UnkE2 = 0;
        pEm->Routine_2++;
        Spd_get2(pEm, pKan, pSeq, 0);
    case 3:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x40);
        if (pEm->Move_cnt == 14) {
            Spd_get2(pEm, pKan, pSeq, 0);
            pEm->Be_flg &= 0xFFFF7FFF;
        }
        if (pEm->Move_cnt >= 6 && pEm->Move_cnt < 14) {
            pEm->Pos_x = EM22_W(pEm)->Unk2C
                + (EM22_W(pEm)->Unk0A - EM22_W(pEm)->Unk2C) * Em22_jump_tbl[pEm->Move_cnt - 6] / 31;
            pEm->Pos_z = EM22_W(pEm)->Unk2E
                + (EM22_W(pEm)->Unk0C - EM22_W(pEm)->Unk2E) * Em22_jump_tbl[pEm->Move_cnt - 6] / 31;
        } else {
            Spd_get2(pEm, pKan, pSeq, 0);
            if (pEm->Move_cnt == 0) {
                pEm->Spd_z = 0;
                pEm->Spd_x = 0;
            }
        }
        if (pEm->Move_cnt == 11) {
            EM22_W(pEm)->Unk08 = 0;
            Snd_se_enem(4, pEm);
        }
        EM22_W(pEm)->Old_spd_x = pEm->Spd_x;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->UnkDA = (Rnd_l() & 0x3F) + 15;
            EM22_W(pEm)->Unk08 = 0;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM22_W(pEm)->Unk03 = (Rnd_l() & 0x3F) + 20;
            EM22_W(pEm)->Old_spd_x = 0;
        }
        if ((ULONG)(pEm->Move_cnt - 6) >= 9)
            Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 5) {
            EM22_W(pEm)->Unk2C = pEm->Pos_x;
            EM22_W(pEm)->Unk2E = pEm->Pos_z;
        }
        if (pEm->Water) {
            if (pEm->Water < pEm->pSin_parts_ptr[0].Workm.t[1] && !pEm->UnkE2) {
                pEm->UnkE2 = 1;
                func_80108AAC(pEm);
            } else if (pEm->Water < pEm->pSin_parts_ptr[16].Workm.t[1] && !pEm->UnkE2) {
                pEm->UnkE2 = 1;
                func_80108AAC(pEm);
            }
        }
    }
}

/* think routine 8 */
void func_80103B34(ENEMY_WORK *pEm)
{
    pEm->Unk16A = 0;
}

/* move routine 8 */
void func_80103B3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1D, 0, 7);
        EM22_W(pEm)->Unk26 = (Rnd_l() & 0x1F) + 120;
        pEm->Routine_2++;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2++;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
        if (pEm->Move_cnt == 10)
            Snd_se_enem(0, pEm);
        break;
    case 2:
        if (Rnd_l() & 1)
            pEm->UnkDA = Rnd_l() & 0x1F;
        else
            pEm->UnkDA = 0;
        EM22_W(pEm)->Unk19 = 0;
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        break;
    }
    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, (Rnd_l() & 3) << 4);
    Add_speedXZ(pEm, 0);
}

/* think routine 9 */
void func_80103CB4(ENEMY_WORK *pEm)
{
}

/* move routine 9 */
void func_80103CBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM22_W(pEm)->Unk08 = 1;
        Snd_se_enem(12, pEm);
        if (EM22_W(pEm)->Unk29)
            EM_SET_MOVE(pEm, 0x18, 0, 7);
        else
            EM_SET_MOVE(pEm, 0x17, 0, 7);
        EM22_W(pEm)->Unk03 = (Rnd_l() & 0x1F) + 30;
        pEm->Routine_2++;
    case 1:
        EM22_W(pEm)->Old_spd_x = 0;
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200) || --EM22_W(pEm)->Unk03 == 0) {
            if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200) && (LONG)pEm->Unk154 > 1200
                && (LONG)pEm->Unk154 < 4600 && (EM22_W(pEm)->Unk28 || pPl->Routine_1 == 5))
                EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            else
                EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    Add_speedXZ(pEm, 0);
}

/* think routine 10 */
void func_80103E38(ENEMY_WORK *pEm)
{
}

/* move routine 10: dash attack */
void func_80103E40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    HIT_AREA Area;

    switch (pEm->Routine_2) {
    case 0:
        if (EM22_W(pEm)->Unk29)
            EM_SET_MOVE(pEm, 0x1A, 0, 7);
        else
            EM_SET_MOVE(pEm, 0x19, 0, 7);
        EM22_W(pEm)->Unk0A = pPl->Pos_x;
        EM22_W(pEm)->Unk0C = pPl->Pos_z;
        EM22_W(pEm)->Unk08 = 1;
        pEm->UnkE2 = 0;
        pEm->UnkDA = 0;
        EM22_W(pEm)->Unk17 = 0;
        pEm->Routine_2++;
        pEm->Be_flg &= 0xFFFF7FFF;
        Snd_se_enem(2, pEm);
    case 1:
        if (EM22_W(pEm)->Unk17 == 11)
            Snd_se_enem(9, pEm);
        if (EM22_W(pEm)->Unk17 == 18) {
            EM22_W(pEm)->Unk08 = 0;
            Snd_se_enem(4, pEm);
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->UnkDA = (Rnd_l() & 0x1F) + 30;
            EM22_W(pEm)->Unk02 = 0;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM22_W(pEm)->Unk03 = (Rnd_l() & 0x3F) + 150;
        }
        Add_speedXZ(pEm, 0);
        if (++EM22_W(pEm)->Unk17 < 18 && __builtin_abs(Pl_work.nFloor - pEm->nFloor) < 2) {
            Sv.vx = 1800;
            Sv.vy = 0;
            Sv.vz = 1400;
            Sv.pad = 1400;
            Area.pPos = WORKM_POS_V(pEm->pSin_parts_ptr);
            Area.pBox = &Sv.vx;
            Area.Dir = pEm->Cdir_y;
            Area.Id = 0x82;
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        }
        EM22_W(pEm)->Old_spd_x = pEm->Spd_x;
        Sv.vx = 400;
        Sv.vy = 0;
        Sv.vz = 0;
        Sv.pad = 800;
        EM22_W(pEm)->Unk20 += 5;
        if (!pPl->Damage_cnt && func_80044420(&pEm->pSin_parts_ptr[9].Workm, &Sv, pPl)) {
            if (!pEm->UnkDA) {
                Pad_vib_set0(7, 0);
                Pad_vib_set1(3, 250, 4);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl) + 2;
                pPl->Routine_2 = 0;
                Snd_se_enem(3, pEm);
                pEm->UnkDA++;
            } else if (++pEm->UnkDA < 10) {
                Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, 0);
            }
        }
    }
    if (pEm->Water) {
        if (pEm->Water < pEm->pSin_parts_ptr[0].Workm.t[1] && !pEm->UnkE2) {
            pEm->UnkE2 = 1;
            func_80108AAC(pEm);
        } else if (pEm->Water < pEm->pSin_parts_ptr[16].Workm.t[1] && !pEm->UnkE2) {
            pEm->UnkE2 = 1;
            func_80108AAC(pEm);
        }
    }
}

/* think routine 11 */
void func_801041C8(ENEMY_WORK *pEm)
{
}

/* move routine 11: climb along the wall */
void func_801041D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = G_PLAYER;
    VECTOR V;
    LONG Ret;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        i = 0;
        Ret = 1;
        switch (func_80103110(pEm, EM22_W(pEm)->Unk29)) {
        case 0:
            for (i = 0; i < 30 && Ret; i++) {
                if (!G.Sca_list[i]) {
                    i = 30;
                    break;
                }
                if (G.Sca_list[i]->X0 + 3500 < pEm->Pos_x && pEm->Pos_x < G.Sca_list[i]->X1 + 1900) {
                    V.vx = pEm->Pos_x - 2700;
                    V.vz = G.Sca_list[i]->Z0 - 300;
                    Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                    if ((ULONG)(__builtin_abs(pEm->Pos_z - V.vz) - 4001) < 999 && !Ret) {
                        func_8010316C(pEm, 0, &V);
                        EM22_W(pEm)->Unk0C = V.vz - 800;
                    } else {
                        Ret = 1;
                    }
                }
            }
            break;
        case 1:
            for (; i < 30 && Ret; i++) {
                if (!G.Sca_list[i]) {
                    i = 30;
                    break;
                }
                if (G.Sca_list[i]->Z0 + 3500 < pEm->Pos_z && pEm->Pos_z < G.Sca_list[i]->Z1 + 1900) {
                    V.vx = G.Sca_list[i]->X0 - 300;
                    V.vz = pEm->Pos_z - 2700;
                    Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                    if ((ULONG)(__builtin_abs(pEm->Pos_x - V.vx) - 4001) < 999 && !Ret) {
                        func_8010316C(pEm, 1, &V);
                        EM22_W(pEm)->Unk0A = V.vx - 800;
                    } else {
                        Ret = 1;
                    }
                }
            }
            break;
        case 2:
            for (; i < 30 && Ret; i++) {
                if (!G.Sca_list[i]) {
                    i = 30;
                    break;
                }
                if (G.Sca_list[i]->X0 + 3500 < pEm->Pos_x && pEm->Pos_x < G.Sca_list[i]->X1 + 1900) {
                    V.vx = pEm->Pos_x - 2700;
                    V.vz = G.Sca_list[i]->Z1 + 300;
                    Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                    if ((ULONG)(__builtin_abs(pEm->Pos_z - V.vz) - 4001) < 999 && !Ret) {
                        func_8010316C(pEm, 0, &V);
                        EM22_W(pEm)->Unk0C = V.vz + 800;
                    } else {
                        Ret = 1;
                    }
                }
            }
            break;
        case 3:
            for (; i < 30 && Ret; i++) {
                if (!G.Sca_list[i]) {
                    i = 30;
                    break;
                }
                if (G.Sca_list[i]->Z0 + 3500 < pEm->Pos_z && pEm->Pos_z < G.Sca_list[i]->Z1 - 3500) {
                    V.vx = G.Sca_list[i]->X1 + 300;
                    V.vz = pEm->Pos_z - 2700;
                    Ret = Sca_ck_line(POS_V(pEm), &V, -1, 3);
                    if ((ULONG)(__builtin_abs(pEm->Pos_x - V.vx) - 4001) < 999 && !Ret) {
                        func_8010316C(pEm, 1, &V);
                        EM22_W(pEm)->Unk0A = V.vx + 800;
                    } else {
                        Ret = 1;
                    }
                }
            }
            break;
        }
        if (i == 30) {
            EM22_W(pEm)->Old_spd_x = 0;
            Snd_se_enem(14, pEm);
            EM_SET_RTN(pEm, 1, 7, 2, 0);
            return;
        }
        if (EM22_W(pEm)->Unk29)
            EM_SET_MOVE(pEm, 0x1C, 0, 5);
        else
            EM_SET_MOVE(pEm, 0x1B, 0, 5);
        pEm->Routine_2++;
        Snd_se_enem(13, pEm);
        EM22_W(pEm)->Unk04 = pEm->Pos_x;
        EM22_W(pEm)->Unk06 = pEm->Pos_z;
        EM22_W(pEm)->Unk1C = pEm->Pos_y;
        break;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y = EM22_W(pEm)->Unk00;
        if (pEm->Move_cnt == 11 && (Rnd_l() & 3))
            EM_SET_RTN(pEm, 1, 9, 0, 0);
        pEm->Pos_x = EM22_W(pEm)->Unk04
            + (EM22_W(pEm)->Unk2C - EM22_W(pEm)->Unk04) * Em22_jump_tbl[pEm->Move_cnt] / 32;
        pEm->Pos_z = EM22_W(pEm)->Unk06
            + (EM22_W(pEm)->Unk2E - EM22_W(pEm)->Unk06) * Em22_jump_tbl[pEm->Move_cnt] / 32;
        EM22_W(pEm)->Old_spd_x = pEm->Spd_x;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM22_W(pEm)->Old_spd_x = 0;
            EM22_W(pEm)->Unk29 = 1 - EM22_W(pEm)->Unk29;
            if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200) && (LONG)pEm->Unk154 > 1200
                && (LONG)pEm->Unk154 < 4600 && (EM22_W(pEm)->Unk28 || pPl->Routine_1 == 5)) {
                EM_SET_RTN(pEm, 1, 0xA, 0, 0);
                return;
            }
            if (Rnd_l() & 1) {
                Snd_se_enem(14, pEm);
                EM_SET_RTN(pEm, 1, 7, 2, 0);
                return;
            }
            pEm->UnkDA = (Rnd_l() & 0x3F) + 120;
            EM_SET_RTN(pEm, 1, 9, 0, 0);
            EM22_W(pEm)->Unk03 = (Rnd_l() & 0x3F) + 150;
        }
        break;
    }
}

/* think routine 12 */
void func_80104864(ENEMY_WORK *pEm)
{
}

/* move routine 12: holding the player (bleeding) */
void func_8010486C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        if (!(Rnd_l() & 0x1F)) {
            Sv.vx = Rnd_l() & 0x1F;
            Sv.vy = 0;
            Sv.vz = Rnd_l() & 0x1F;
            Esp_call(0x500, (USHORT)pEm->Cdir_y | 0x10000000, &pPl->pSin_parts_ptr[1].Workm, &Sv, 0);
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        Add_speedXZ(pEm, 0);
    }
}

/* think routine 13 */
void func_80104974(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;

    if (pEm->Routine_2 == 1 && pEm->UnkDA >= 16 && (LONG)pEm->Unk154 < 3000 && pPl->Routine_0 == 5
        && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300) && func_80108CB8(pEm, 0xF01))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
}

/* move routine 13: grab and hold the player */
void func_80104A10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    HIT_AREA Area;

    func_80101914(pEm, pPl);
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x10, 0, 7);
        pEm->Routine_2 = 1;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->UnkDA = 0;
        EM22_W(pEm)->Unk17 = 0;
        EM22_W(pEm)->Unk20 = 800;
        EM22_W(pEm)->Unk1E = 0;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM22_W(pEm)->Unk08 = 0;
            if (EM22_W(pEm)->Unk16 != 0xFF)
                EM22_W(pEm)->Unk03 = (Rnd_l() & 0x3F) + 150;
            else
                EM22_W(pEm)->Unk03 = (Rnd_l() & 0x3F) + 30;
            pEm->UnkDA = (Rnd_l() & 0x3F) + 150;
            EM22_W(pEm)->Unk1B = 0;
            EM22_W(pEm)->Unk02 = 0;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            break;
        }
        if (++EM22_W(pEm)->Unk17 < 18 && __builtin_abs(Pl_work.nFloor - pEm->nFloor) < 2) {
            Sv.vx = 2000;
            Sv.vz = 1000;
            Sv.vy = 0;
            Sv.pad = 1400;
            Area.pPos = WORKM_POS_V(&pEm->pSin_parts_ptr[8]);
            Area.pBox = &Sv.vx;
            Area.Dir = pEm->Cdir_y;
            if (!pEm->Water) {
                Area.Id = 0xA2;
                if ((Rnd_l() & 3) == 1)
                    Area.Id = 0x81;
            } else {
                Area.Id = 0x82;
            }
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        }
        if (EM22_W(pEm)->Unk17 >= 19 && EM22_W(pEm)->Unk17 < 30) {
            Sv.vx = 400;
            Sv.vz = 0;
            Sv.vy = 0;
            Sv.pad = EM22_W(pEm)->Unk20;
            if (func_80044420(&pEm->pSin_parts_ptr[11].Workm, &Sv, pPl) && !pPl->Damage_cnt && !pEm->UnkDA) {
                Pad_vib_set0(7, 0);
                Pad_vib_set1(3, 250, 4);
                Pl_life_down(15, 1);
                pPl->pEnemy_ptr = pEm;
                EM_RTN(pPl) = (Cdir_ck(pEm, pPl) << 24) | 5;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                pPl->Damage_cnt |= 0x80;
                pEm->UnkDA = 1;
            }
        }
        if (pEm->UnkDA && ++pEm->UnkDA < 5) {
            Sv.vx = 0;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, 0);
        }
    }
    if (pEm->UnkDA)
        Goto00(pEm, pEm->Pos_x, pEm->Pos_z, 16);
    else
        Goto00(pEm, EM22_W(pEm)->Unk0A, EM22_W(pEm)->Unk0C, 16);
    Add_speedXZ(pEm, 0);
}

/* think routine 14 */
void func_80104D78(ENEMY_WORK *pEm)
{
}

/* move routine 14 */
void func_80104D80(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Dist;
    LONG Dir;
    LONG Side;

    Dist = pEm->Unk154;
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x180);
    Side = func_80101A30(pEm);
    switch (pEm->Routine_2) {
    case 0:
        if (pEm->Routine_3 == 3)
            pEm->Routine_3 = Rnd_l() & 2;
        if (pEm->Routine_3 & 2)
            EM_SET_MOVE(pEm, 0x1F, 0, 7);
        else
            EM_SET_MOVE(pEm, 0x1E, 0, 7);
        pEm->Routine_2++;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (Dist < 3000 && !Dir && (pEm->Status_flg & 0x2000)) {
                if (func_80101988(pEm))
                    EM_SET_RTN(pEm, 1, 3, 0, 0);
                else if (func_80101A30(pEm))
                    EM_SET_RTN(pEm, 1, 4, 0, 0);
            } else if ((pEm->Status_flg & 0x2000) && (Rnd_l() & 1) && Side && Dist < 5000
                       && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100) && func_80108CB8(pEm, 0x401)
                       && func_80101A30(pEm)) {
                EM_SET_RTN(pEm, 1, 4, 0, 0);
                return;
            } else {
                EM_SET_RTN(pEm, 1, 2, 0, 0);
                if (pEm->Status_flg & 0x2000) {
                    EM22_W(pEm)->Unk1B = 10;
                    EM22_W(pEm)->Unk19 = 0;
                    EM22_W(pEm)->Unk02 = 1;
                    if (pPl->Life < 71 && pEm->Id == 0x24 && Dist < 5400
                        && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100))
                        EM_SET_RTN(pEm, 1, 0xD, 0, 0);
                }
            }
        }
    }
    Add_speedXZ(pEm, 0);
}

/* is there room to jump to the side? (Routine_3: 1 or 2 when one side is free) */
LONG func_80104FF0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    MATRIX M;
    VECTOR V0 = { 0xC1C, -100, 0xAF0, 0 };
    VECTOR V1 = { -0xC1C, -100, 0xAF0, 0 };

    if ((Rnd_l() >> (Rnd_l() & 3)) & 3)
        return 0;
    if (pPl->Type == 1 && (Rnd_l() & 3))
        return 0;
    if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400))
        return 0;
    RotMatrixY(pEm->Cdir_y, &M);
    ApplyMatrixLV(&M, &V1, &V1);
    ApplyMatrixLV(&M, &V0, &V0);
    V1.vx += pEm->Pos_x;
    V1.vy += pEm->Unk122;
    V1.vz += pEm->Pos_z;
    V0.vx += pEm->Pos_x;
    V0.vy += pEm->Unk122;
    V0.vz += pEm->Pos_z;
    Sca_ck_line(POS_V(pEm), &V1, 0x200, 2);
    return Sca_ck_line(POS_V(pEm), &V0, 0x200, 2) ^ 3;
}

/* think routine 15 */
void func_801051B8(ENEMY_WORK *pEm)
{
}

/* move routine 15: throw (grab from the front) */
void func_801051C0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SHORT X0;
    SHORT Z0;
    SHORT Rate;
    LONG Dir;
    LONG Unused[6];

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x20, 0, 7);
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->UnkDA = 0;
        EM22_W(pEm)->Unk17 = 0;
        EM22_W(pEm)->Unk20 = 800;
        EM22_W(pEm)->Unk1E = 0;
        func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
        pPl->pEnemy_ptr = pEm;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Move_cnt >= 6 && pEm->Move_cnt < 26)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x18);
        if (pEm->Move_cnt == 26) {
            if (pPl->Routine_0 != 6 && (LONG)pEm->Unk154 < 2600
                && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100)) {
                Dir = Cdir_ck(pEm, pPl);
                pPl->pEnemy_ptr = pEm;
                pPl->Damage_cnt |= 0x80;
                pEm->Be_flg |= 0x8800;
                EM_RTN(pPl) = (Dir << 24) | 6;
                pPl->Be_flg |= 0xA800;
                Set_base_pos(pPl, pKan, pSeq, 0);
            } else {
                EM_SET_MOVE(pEm, 0x21, 0, 7);
                Joint_move(pEm, pKan, pSeq, 0x200);
                EM_RTN23(pEm) = 3;
                break;
            }
        }
        if (pEm->Move_cnt == 36 || pEm->Move_cnt == 50 || pEm->Move_cnt == 68)
            Snd_se_enem(15, pEm);
        if (pEm->Move_cnt >= 26) {
            Null_pos_set(pEm, pKan, pSeq, 0);
            X0 = pEm->Pos_x;
            Z0 = pEm->Pos_z;
        }
        if (pEm->Move_cnt == 26) {
            Pad_vib_set0(3, 4);
            Pad_vib_set0(3, 0x10);
            Pad_vib_set0(5, 0x1A);
            Pad_vib_fade_set(12, 0xBC, 0x40, 4);
            Pad_vib_fade_set(8, 0xB0, 0x14, 0x10);
            Pad_vib_fade_set(12, 0x98, 0xD4, 0x18);
            Pad_vib_fade_set(28, 0xD4, 0, 0x24);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2++;
        if (pEm->Move_cnt >= 27 && pEm->Move_cnt < 34) {
            Rate = 34 - pEm->Move_cnt;
            pEm->Pos_x -= (pEm->Pos_x - X0) * Rate >> 3;
            pEm->Pos_z -= (pEm->Pos_z - Z0) * Rate >> 3;
        }
        break;
    case 2:
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM22_W(pEm)->Unk1B = 30;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        break;
    }
}

/* Em22_Scenario */
void func_8010555C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Type & 0x100)
        func_80016600(pEm, pKan, pSeq);
    else
        Em22_sce_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* scenario routine 0: drop from the ceiling */
void func_801055B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Routine_2++;
    case 1:
        if (pEm->Type & 0x4000)
            break;
        pEm->Be_flg &= ~0x30;
        pEm->Routine_2++;
        Snd_se_enem(2, pEm);
        Snd_se_stad(0x10204, POS_V(pEm), 0, 0);
        pEm->UnkDA = 0;
        pEm->Pos_y = 1800;
    case 2:
        func_80106118(pEm);
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Pos_y = pEm->Move_cnt < 18 ? pEm->Pos_y - 100 : pEm->Pos_y;
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM22_W(pEm)->Unk02 = 1;
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            pEm->Pos_y = 0;
            pEm->Unk122 = 0;
            pEm->Type = 0;
            pEm->Be_flg &= ~0x2000;
            pEm->Damage_cnt &= 0x7F;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* scenario routine 1: jump down to (Dest_x, Add_dir, Dest_z) */
void func_80105730(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Dx;
    LONG Dz;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Routine_2++;
    case 1:
        if (pEm->Type & 0x4000)
            break;
        pEm->Be_flg &= ~0x30;
        pEm->Type &= 0xEFFF;
        pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);
        Dx = pEm->Dest_x - pEm->Pos_x;
        Dz = pEm->Dest_z - pEm->Pos_z;
        pEm->Timer0 = (SquareRoot0(Dx * Dx + Dz * Dz) << 3) / 5000;
        pEm->Unk14A = (pEm->Add_dir - pEm->Pos_y) / 20;
        pEm->Routine_2++;
        Snd_se_enem(2, pEm);
        pEm->UnkDA = 0;
        EM22_W(pEm)->Unk08 = 1;
    case 2:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Pos_y = pEm->Move_cnt < 20 ? pEm->Pos_y + pEm->Unk14A : pEm->Pos_y;
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM22_W(pEm)->Unk08 = 0;
            pEm->Type = 0;
            pEm->Pos_y = pEm->Add_dir;
            pEm->Be_flg &= ~0x2000;
            pEm->Damage_cnt &= 0x7F;
        }
        pEm->Spd_x = (SHORT)(pEm->Spd_x * pEm->Timer0) >> 3;
        pEm->Spd_z = (SHORT)(pEm->Spd_z * pEm->Timer0) >> 3;
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 27)
            Snd_se_enem(4, pEm);
    }
}

/* scenario routine 2: turning jump down to (Dest_x, Add_dir, Dest_z) */
void func_8010598C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Dx;
    LONG Dz;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0xC, 0);
        pEm->Damage_cnt |= 0x80;
        pEm->Routine_2++;
    case 1:
        if (pEm->Type & 0x4000)
            break;
        pEm->Be_flg = (pEm->Be_flg & ~0x30) | 0x8000;
        pEm->Type &= 0xEFFF;
        pEm->Unk14C = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);
        pEm->Unk14C = (pEm->Unk14C - pEm->Cdir_y) & 0xFFF;
        if (pEm->Unk14C > 0x800)
            pEm->Unk14C -= 0x1000;
        Dx = pEm->Dest_x - pEm->Pos_x;
        Dz = pEm->Dest_z - pEm->Pos_z;
        pEm->Unk14C /= 10;
        pEm->Timer0 = (SquareRoot0(Dx * Dx + Dz * Dz) << 3) / 5000;
        pEm->Unk14A = (pEm->Add_dir - pEm->Pos_y) / 10;
        pEm->Routine_2++;
        Snd_se_enem(2, pEm);
        pEm->UnkDA = 0;
        EM22_W(pEm)->Unk08 = 1;
        pEm->Unk122 = pEm->Add_dir;
        Spd_get2(pEm, pKan, pSeq, 0);
    case 2:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt < 22) {
            pEm->Cdir_y += pEm->Unk14C;
            pEm->Pos_y += pEm->Unk14A;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Type = 0;
            EM22_W(pEm)->Unk08 = 0;
            pEm->Pos_y = pEm->Add_dir;
            pEm->Be_flg &= ~0x6000;
            pEm->Be_flg &= 0xFFFF7FFF;
            pEm->Damage_cnt &= 0x7F;
        }
        pEm->Spd_x = (SHORT)(pEm->Spd_x * pEm->Timer0) >> 3;
        pEm->Spd_z = (SHORT)(pEm->Spd_z * pEm->Timer0) >> 3;
        Add_speedXZ(pEm, 0);
    }
}

/* scenario routine 3: fixed jump path (4 points) */
void func_80105C64(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Px[4] = { -14520, -13585, -13480, -10000 };
    SHORT Pz[4] = { -7560, -4411, -1860, 0 };
    SHORT Py[4] = { -3600, 0, 0, -2000 };
    SHORT Jump[4] = { 1, 1, 0, 1 };
    SHORT Time[4] = { 20, 25, 10, 30 };
    LONG Dx;
    LONG Dz;

    switch (pEm->Routine_2) {
    case 0:
        if (pEm->Type & 0x4000)
            break;
        pEm->Unk122 = -1800;
        pEm->UnkDA = 0;
        pEm->Be_flg &= ~0x30;
        pEm->Type &= 0xEFFF;
        pEm->Routine_2++;
    case 1:
        pEm->Dest_x = Px[pEm->UnkDA];
        pEm->Dest_z = Pz[pEm->UnkDA];
        pEm->Add_dir = Py[pEm->UnkDA];
        if (Jump[pEm->UnkDA]) {
            Dx = pEm->Dest_x - pEm->Pos_x;
            Dz = pEm->Dest_z - pEm->Pos_z;
            EM_SET_MOVE(pEm, 4, 0, 7);
            pEm->Timer0 = (SHORT)((SquareRoot0(Dx * Dx + Dz * Dz) << 3) / 5000) * 35;
            pEm->Timer0 /= Time[pEm->UnkDA];
            pEm->Unk14A = (pEm->Add_dir - pEm->Pos_y) / Time[pEm->UnkDA];
        } else {
            EM_SET_MOVE(pEm, 5, 0, 7);
        }
        pEm->UnkE2 = Time[pEm->UnkDA];
        pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);
        pEm->Routine_2++;
        if (pEm->UnkDA == 0 || pEm->UnkDA == 3)
            Snd_se_enem(2, pEm);
        EM22_W(pEm)->Unk08 = 1;
    case 2:
        if (pEm->UnkDA == 1 && pEm->Move_cnt == 15)
            pEm->Unk122 = 0;
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Jump[pEm->UnkDA])
            pEm->Pos_y += pEm->Unk14A;
        else
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x100) || --pEm->UnkE2 == 0) {
            if (pEm->UnkDA == 1)
                Snd_se_enem(4, pEm);
            pEm->Pos_y = pEm->Add_dir;
            if (++pEm->UnkDA < 4) {
                pEm->Routine_2--;
            } else {
                pEm->Be_flg |= 0x30;
                pEm->Type |= 0x8000;
            }
        }
        if (Jump[pEm->UnkDA]) {
            pEm->Spd_x = (SHORT)(pEm->Spd_x * pEm->Timer0) >> 3;
            pEm->Spd_z = (SHORT)(pEm->Spd_z * pEm->Timer0) >> 3;
        }
        Add_speedXZ(pEm, 0);
        func_80109058(pEm);
    }
}

/* dust from random parts while landing */
void func_80106118(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG Dir;
    LONG n;
    SVECTOR Sv;

    if (pEm->Move_cnt < 21) {
        n = (20 - pEm->Move_cnt) / 8;
        while (n) {
            pP = pEm->pSin_parts_ptr;
            n--;
            pP += Rnd_l() & 0xF;
            Dir = ((Rnd_l() & 3) << 10) + 0x1000;
            Sv.vx = 0;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x91C, (Dir << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
        }
    }
}

/* RE2 Em_damage (Em22_Damage) */
void func_801061FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM22_FREE *pFp;
    LONG Flg;
    LONG Bit;

    pEm->Unk132 &= 0x3FFF;
    Flg = 1;
    EM22_W(pEm)->Unk03 = 0;
    EM22_W(pEm)->Unk1B = 1;
    EM22_W(pEm)->Unk02 = 1;
    if ((ULONG)(pEm->Unk12C - 6) < 4)
        Flg = (Rnd_l() & 3) != 0;
    if (!EM22_W(pEm)->Unk13) {
        Flg = 0;
        EM22_W(pEm)->Unk2B = 0;
        EM22_W(pEm)->Unk12 = 1;
    }
    if (pEm->Unk12C == 14 || pEm->Unk12C == 15) {
        pFp = EM22_W(pEm);
        if (++EM22_W(pEm)->Unk2B >= 3) {
            EM22_W(pEm)->Unk2B = 0;
        } else if ((pEm->Unk1C4 & 0xF) == 1) {
            pFp->Unk11 = 0;
            EM22_W(pEm)->Unk10 = 0;
            pEm->Timer0 = 1;
            EM_RTN(pEm) = pEm->Unk1C4;
            func_8010649C(pEm);
            func_80100680(pEm, pKan, pSeq);
            return;
        }
        goto common;
    }
    if (Em22_dm_mask[pEm->Unk12C - 1] && Flg) {
        Bit = pEm->Unk132 & 0xC;
        Bit >>= 1;
        if (!Bit)
            Bit = 1;
        if (Bit & Em22_dm_mask[pEm->Unk12C - 1])
            goto common;
    }
    if ((pEm->Unk1C4 & 0xF) == 1 && EM22_W(pEm)->Unk12) {
        EM22_W(pEm)->Unk11 = 0;
        EM22_W(pEm)->Unk10 = 0;
        pEm->Timer0 = 1;
        EM_RTN(pEm) = pEm->Unk1C4;
        func_8010649C(pEm);
        func_80100680(pEm, pKan, pSeq);
        EM22_W(pEm)->Unk12 ^= 1;
        return;
    }
common:
    func_8010649C(pEm);
    if (!EM22_W(pEm)->Unk10) {
        func_80106774(pEm, pKan, pSeq);
        pEm->pSin_parts_ptr[11].Poly_rgb = 0x808080;
        pEm->pSin_parts_ptr[12].Poly_rgb = 0x808080;
        pEm->pSin_parts_ptr[13].Poly_rgb = 0x808080;
    }
    pEm->Routine_2 = EM22_W(pEm)->Unk10;
    pEm->Routine_3 = EM22_W(pEm)->Unk11;
    Em22_dm_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    pEm->Unk1C4 = EM_RTN(pEm);
    if (pEm->Id == 0x24) {
        if (*pEm->pNow_seq & 0x800)
            pEm->Unk16A = 1;
        func_80108DAC(pEm);
    }
}

/* blood spray while Timer0 runs, from the parts hit */
#define EM22_BLOOD_RND()                    \
    Sv.vx = (Rnd_l() & 0x3F) - 32;            \
    Sv.vy = (Rnd_l() & 0x3F) - 32;            \
    Sv.vz = (Rnd_l() & 0x3F) - 32;            \
    Dir = ((Rnd_l() & 3) << 9) + 0xE00

void func_8010649C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    LONG Dir;
    LONG Id;

    EM22_BLOOD_RND();
    Id = 0;
    if (pEm->Unk132 & 0x200)
        Id = 5;
    if (pEm->Timer0) {
        switch (pEm->Routine_1) {
        case 0:
            if (!(EM22_W(pEm)->Unk0F & 4)) {
                Esp_call((Id << 24) | 0x700, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[17].Workm, &Sv, 0);
                EM22_BLOOD_RND();
                Esp_call(Id << 24, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[16].Workm, &Sv, 0);
            }
            break;
        case 1:
            Esp_call((Id << 24) | 0x700, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[1].Workm, &Sv, 0);
            EM22_BLOOD_RND();
            Esp_call(Id << 24, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[4].Workm, &Sv, 0);
            break;
        case 2:
            Esp_call(Id << 24, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[3].Workm, &Sv, 0);
            EM22_BLOOD_RND();
            Esp_call((Id << 24) | 0x700, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[0].Workm, &Sv, 0);
            break;
        case 3:
            if (!(EM22_W(pEm)->Unk0F & 1))
                Esp_call(Id << 24, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[5].Workm, &Sv, 0);
            break;
        }
        pEm->Timer0--;
    }
}

/* damage reaction setup */
void func_80106774(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (EM22_W(pEm)->Unk08 || D_80109488[pEm->Unk12C - 1]) {
        EM22_W(pEm)->Unk10 = pEm->Routine_2 = 2;
        EM22_W(pEm)->Unk11 = 0;
        EM22_W(pEm)->Unk08 = 0;
    } else {
        EM22_W(pEm)->Unk10 = pEm->Routine_2 = 1;
        EM22_W(pEm)->Unk11 = 0;
    }
    EM22_W(pEm)->Unk22 = (pEm->Unk132 >> 4) & 1;
    if ((pEm->Unk132 & 3) == 1 && EM22_W(pEm)->Unk16 < 3)
        EM22_W(pEm)->Unk16++;
    if (pEm->Id != 0x24 && !(pEm->Status_flg & 2) && (pEm->Unk132 & 0x200))
        func_801068F4(pEm);
    if (!(pEm->Status_flg & 1) && (pEm->Unk132 & 0x100))
        func_80106A6C(pEm);
    if (!(pEm->Status_flg & 4) && (pEm->Unk132 & 0x800))
        func_80106BDC(pEm);
    if (pEm->Unk132 & 0x400)
        func_80106D00(pEm);
    func_8002646C_(pEm, 400, 40, pEm->Col.Unk39);
}

/* hit by fire: burn the parts of the hit area */
void func_801068F4(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR Sv;

    pEm->Unk178[2] = 0x1000;
    pEm->Unk17E = 0x40;
    pEm->Unk178[0] = 0;
    pEm->Unk178[1] = 0;
    pEm->Be_flg |= 8;
    switch (pEm->Unk132 & 3) {
    case 1:
        pP = &pEm->pSin_parts_ptr[15];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[16];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[19];
        pP->UnkA2 |= 0x40;
        break;
    case 2:
        pP = &pEm->pSin_parts_ptr[0];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[17];
        pP->UnkA2 |= 0x40;
        break;
    case 3:
        pP = &pEm->pSin_parts_ptr[1];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 0x40;
        break;
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1803, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
    if (EM22_W(pEm)->Unk16 != 0xFF)
        EM22_W(pEm)->Unk16 = 0xFF;
}

/* hit by acid: effects on three random parts of the hit area */
void func_80106A6C(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR Sv;
    LONG i;

    for (i = 0; i < 3; i++) {
        switch (pEm->Unk132 & 3) {
        case 1:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 14];
            break;
        case 2:
            pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
            break;
        case 3:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 5];
            break;
        }
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(0x205, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
    }
    for (i = 0; i < 20; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
    }
}

/* hit by a shock: effect on a random part of the hit area */
void func_80106BDC(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR Sv;

    switch (pEm->Unk132 & 3) {
    case 1:
        pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 14];
        break;
    case 2:
        pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
        break;
    case 3:
        pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 5];
        break;
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1503, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
}

/* RE3 dummy */
void func_80106D00(ENEMY_WORK *pEm)
{
}

/* damage routine 1: flinch */
void func_80106D08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Side;
    LONG Spd;

    Side = func_80101A30(pEm);
    switch (pEm->Routine_3) {
    case 0:
        if ((pEm->Unk132 & 3) == 1 && EM22_W(pEm)->Unk16 == 3) {
            EM22_W(pEm)->Unk16 = 0xFF;
            EM22_W(pEm)->Unk1B = -6;
            EM_SET_MOVE(pEm, 0x11, 0, 7);
            EM22_W(pEm)->Old_spd_x = 0;
        } else if (EM22_W(pEm)->Unk22) {
            EM22_W(pEm)->Old_spd_x = 200;
            EM_SET_MOVE(pEm, 7, 0, 7);
        } else {
            EM22_W(pEm)->Old_spd_x = -200;
            EM_SET_MOVE(pEm, 6, 0, 7);
        }
        pEm->Timer0 = 5;
        pEm->UnkE2 = 0;
        pEm->Routine_3++;
        Snd_se_enem(6, pEm);
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Spd_x += EM22_W(pEm)->Old_spd_x;
        Spd = EM22_W(pEm)->Old_spd_x;
        if (Spd) {
            if (Spd > 0)
                EM22_W(pEm)->Old_spd_x = Spd - 50;
            else
                EM22_W(pEm)->Old_spd_x = Spd + 50;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM22_W(pEm)->Unk12 ^= 1;
            EM22_W(pEm)->Unk11 = 0;
            EM22_W(pEm)->Unk10 = 0;
            pEm->Be_flg |= 0x80000;
            if ((G.Unk75E8 & 8) && !(Rnd_l() & 1) && EM22_W(pEm)->Unk16 != 0xFF) {
                if (Side && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80) && EM22_W(pEm)->Unk28
                    && func_80108CB8(pEm, 0x401) && (pEm->Status_flg & 0x2000)) {
                    EM_SET_RTN(pEm, 1, 4, 0, 0);
                } else {
                    EM22_W(pEm)->Unk19 = 0;
                    EM_SET_RTN(pEm, 1, 5, 0, 0);
                }
            } else if (Rnd_l() & 7) {
                EM_SET_RTN(pEm, 1, 1, 0, 0);
                EM22_W(pEm)->Unk03 = 0;
            } else {
                EM_SET_RTN(pEm, 1, 2, 0, 0);
                EM22_W(pEm)->Unk1B = 10;
                EM22_W(pEm)->Unk19 = 0;
                EM22_W(pEm)->Unk02 = 1;
            }
        }
    }
    Add_speedXZ(pEm, 0);
    EM22_W(pEm)->Unk11 = pEm->Routine_3;
}

/* damage routine 2: knocked down */
void func_80106F8C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;
    LONG Lim;

    switch (pEm->Routine_3) {
    case 0:
        if (EM22_W(pEm)->Unk22)
            EM_SET_MOVE(pEm, 9, 0, 0xF);
        else
            EM_SET_MOVE(pEm, 8, 0, 0xF);
        pEm->Timer0 = 5;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        pEm->Routine_3++;
        Snd_se_enem(6, pEm);
        EM22_W(pEm)->Unk1E = 0;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            if (!(Rnd_l() & 3))
                pEm->Routine_3++;
            else
                pEm->Routine_3 += 3;
        }
        if (pEm->Move_cnt < 16) {
            pEm->Spd_x += EM22_W(pEm)->Old_spd_x;
            D0 = EM22_W(pEm)->Old_spd_x;
            if (D0 > 0) {
                if (D0 == 1)
                    EM22_W(pEm)->Old_spd_x = 0;
                EM22_W(pEm)->Old_spd_x >>= 1;
            } else {
                EM22_W(pEm)->Old_spd_x = 0;
            }
        }
        if (EM22_W(pEm)->Old_spd_x)
            pEm->Spd_x = EM22_W(pEm)->Old_spd_x;
        else if (!(pEm->Spd_x >>= 1))
            pEm->Spd_x = 1;
        EM22_W(pEm)->Unk1E += 75;
        pEm->Pos_y += EM22_W(pEm)->Unk1E;
        Lim = -(pEm->nFloor * 1800);
        if (Lim < pEm->Pos_y) {
            pEm->Pos_y = Lim;
            if (!(Rnd_l() & 7)) {
                if (EM22_W(pEm)->Unk22 && pEm->Move_cnt == 28)
                    pEm->Routine_3 += 3;
                if (!EM22_W(pEm)->Unk22 && pEm->Move_cnt == 25)
                    pEm->Routine_3 += 3;
            }
        }
        break;
    case 2:
        pEm->UnkE3 = (Rnd_l() & 0x1F) + 7;
        pEm->Routine_3++;
    case 3:
        pEm->Unk132 |= 0x4000;
        if (--pEm->UnkE3 == 0) {
            pEm->Routine_3++;
            break;
        }
        goto snd;
    case 4:
        if (EM22_W(pEm)->Unk22)
            EM_SET_MOVE(pEm, 0xD, 0, 7);
        else
            EM_SET_MOVE(pEm, 0xC, 0, 7);
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Routine_3++;
    case 5:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (EM22_W(pEm)->Unk22) {
            if (pEm->Move_cnt == 27) {
                Snd_se_enem(8, pEm);
                pEm->Unk132 &= 0xBFFF;
            }
        } else if (pEm->Move_cnt == 10) {
            Snd_se_enem(8, pEm);
            pEm->Unk132 &= 0xBFFF;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM22_W(pEm)->Unk12 ^= 1;
            EM22_W(pEm)->Unk11 = 0;
            EM22_W(pEm)->Unk10 = 0;
            pEm->Be_flg |= 0x80000;
            if (Rnd_l() & 7) {
                EM_SET_RTN(pEm, 1, 0, 0, 0);
                EM22_W(pEm)->Unk03 = 0;
                if (EM22_W(pEm)->Unk22 && (Rnd_l() & 1)) {
                    EM22_W(pEm)->Unk19 = 0;
                    EM_SET_RTN(pEm, 1, 5, 0, 0);
                }
            } else {
                EM_SET_RTN(pEm, 1, 2, 0, 0);
                EM22_W(pEm)->Unk1B = 10;
                EM22_W(pEm)->Unk19 = 0;
                EM22_W(pEm)->Unk02 = 1;
            }
            return;
        }
    snd:
        if ((pEm->Damage_cnt & 0x7F) && !pEm->Timer0) {
            pEm->Timer0 = 5;
            Snd_se_enem(6, pEm);
        }
        break;
    }
    if (pEm->Routine_3 != 3)
        Add_speedXZ(pEm, 0);
    if (pEm->Routine_3 != 5)
        func_80109058(pEm);
    EM22_W(pEm)->Unk11 = pEm->Routine_3;
    D0 = pEm->Water;
    if (D0 && !pEm->UnkE2
        && (D0 < pEm->pSin_parts_ptr[0].Workm.t[1] || D0 < pEm->pSin_parts_ptr[16].Workm.t[1])) {
        pEm->UnkE2 = 1;
        func_80108AAC(pEm);
    }
}

static inline void Water_splash(ENEMY_WORK *pEm)
{
    if (pEm->Water && !pEm->UnkE2
        && (pEm->Water < pEm->pSin_parts_ptr[0].Workm.t[1] || pEm->Water < pEm->pSin_parts_ptr[16].Workm.t[1])) {
        pEm->UnkE2 = 1;
        func_80108AAC(pEm);
    }
}
/* RE2 Em_die: routine_0 3 */
void func_80107448(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 40, pEm->Col.Unk39);
        pP = &pEm->pSin_parts_ptr[11];
        pP->Poly_rgb = 0x808080;
        pP = &pEm->pSin_parts_ptr[12];
        pP->Poly_rgb = 0x808080;
        pP = &pEm->pSin_parts_ptr[13];
        pP->Poly_rgb = 0x808080;
        if (pEm->Id != 0x24 && !(pEm->Status_flg & 2) && (pEm->Unk132 & 0x200))
            func_801078B8(pEm);
        if (!(pEm->Status_flg & 1) && (pEm->Unk132 & 0x100)) {
            if (pEm->Id == 0x22)
                func_80107A1C(pEm);
            else
                func_80107B94(pEm);
        }
        if (!(pEm->Status_flg & 4) && (pEm->Unk132 & 0x800))
            func_80107D0C(pEm);
        if (pEm->Unk132 & 0x400) {
            if (pEm->Id == 0x22)
                func_80107E30(pEm);
            else
                func_80108110(pEm);
        }
        if (EM22_W(pEm)->Unk10 == 2 && EM22_W(pEm)->Unk11 < 4) {
            if (EM22_W(pEm)->Unk11 >= 2) {
                if (EM22_W(pEm)->Unk22)
                    EM_SET_MOVE(pEm, 0xF, 0, 3);
                else
                    EM_SET_MOVE(pEm, 0xE, 0, 3);
            }
            EM22_W(pEm)->Unk28 = 0;
        } else if (EM22_W(pEm)->Unk08) {
            if ((pEm->Unk132 >> 4) & 1) {
                EM_SET_MOVE(pEm, 9, 0, 0xF);
                EM22_W(pEm)->Unk28 = 2;
            } else {
                EM_SET_MOVE(pEm, 8, 0, 0xF);
                EM22_W(pEm)->Unk28 = 3;
            }
        } else {
            EM22_W(pEm)->Unk28 = 1;
            EM_SET_MOVE(pEm, 0xA, 0, 0xF);
        }
        Em_dead_flg_on_cand(pEm);
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        pEm->Routine_2++;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Timer0 = 1;
        pEm->Routine_1 = Rnd_l() & 3;
        if (!(Rnd_l() & 3))
            func_8010649C(pEm);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (EM22_W(pEm)->Unk08 && pEm->Col.W0 < 800)
            pEm->Col.D0 = pEm->Col.W0 = pEm->Col.W0 + 50;
        if (++pEm->UnkE3 == 5)
            Snd_se_enem(7, pEm);
        pEm->Col.W0 -= 5;
        if (pEm->Pos_y < -(pEm->nFloor * 1800))
            pEm->Pos_y += 50;
        if (-(pEm->nFloor * 1800) < pEm->Pos_y)
            pEm->Pos_y = -(pEm->nFloor * 1800);
        break;
    case 2:
        if (EM22_W(pEm)->Unk28 >= 2) {
            if (EM22_W(pEm)->Unk28 == 2)
                EM_SET_MOVE(pEm, 0xF, 0, 7);
            else
                EM_SET_MOVE(pEm, 0xE, 0, 7);
        }
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
    func_80109058(pEm);
    Add_speedXZ(pEm, 0);
    if (pEm->Water && !pEm->UnkE2) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Water < pP->Workm.t[1] || pEm->Water < pP[16].Workm.t[1]) {
        pEm->UnkE2 = 1;
        func_80108AAC(pEm);
        }
    }
    if (pEm->Id == 0x24) {
        if (*pEm->pNow_seq & 0x800)
            pEm->Unk16A = 1;
        func_80108DAC(pEm);
    }
}

/* die: blow off a part (Unk132 & 3 picks which), blood spray from it */
void func_801078B8(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;

    pEm->Unk178[2] = 0x1000;
    pEm->Unk17E = 0x60;
    pEm->Unk178[0] = 0;
    pEm->Unk178[1] = 0;
    pEm->Be_flg |= 8;
    switch (pEm->Unk132 & 3) {
    case 1:
        pP = &pEm->pSin_parts_ptr[15];
        pP->UnkA2 |= 0x80;
        pP = &pEm->pSin_parts_ptr[16];
        pP->UnkA2 |= 0x80;
        pP = &pEm->pSin_parts_ptr[19];
        pP->UnkA2 |= 0x80;
        break;
    case 2:
        pP = &pEm->pSin_parts_ptr[0];
        pP->UnkA2 |= 0x80;
        pP = &pEm->pSin_parts_ptr[17];
        pP->UnkA2 |= 0x80;
        break;
    case 3:
        pP = &pEm->pSin_parts_ptr[1];
        pP->UnkA2 |= 0x80;
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 0x80;
        break;
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1503, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
}

/* die: three blood sprays, then tint all parts 0x202F2F */
void func_80107A1C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    LONG i;

    for (i = 0; i < 3; i++) {
        switch (pEm->Unk132 & 3) {
        case 1:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 14];
            break;
        case 2:
            pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
            break;
        case 3:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 5];
            break;
        }
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(0x205, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
    }
    for (i = 0; i < 20; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
        pP->Poly_rgb = 0x202F2F;
    }
}

/* die: three blood sprays, then tint all parts 0x4F4F40 */
void func_80107B94(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    LONG i;

    for (i = 0; i < 3; i++) {
        switch (pEm->Unk132 & 3) {
        case 1:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 14];
            break;
        case 2:
            pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
            break;
        case 3:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 5];
            break;
        }
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(0x205, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
    }
    for (i = 0; i < 20; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
        pP->Poly_rgb = 0x4F4F40;
    }
}

/* die: blood spray from a random part of the side that was hit */
void func_80107D0C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Unk132 & 3) {
    case 1:
        pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 14];
        break;
    case 2:
        pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
        break;
    case 3:
        pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 5];
        break;
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1503, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
}

/* die (id 0x22): the arms / legs given by Unk132 bits 2-3 fall off, then tint all parts 0x202F2F */
void func_80107E30(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG i;

    EM22_W(pEm)->Unk0F = 0;
    switch ((pEm->Unk132 & 0xC) >> 2) {
    case 0:
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
        PW_FREE(pP)[1] = -240;
        PW_FREE(pP)[4] = 0;
        PW_FREE(pP)[2] = 0;
        PW_FREE(pP)[3] = 15;
        PW_FREE(pP)[6] = 0;
        for (i = 6; i < 8; i++) {
            pP = &pEm->pSin_parts_ptr[i];
            pP->Be_flg |= 0xA;
            pP->Poly_rgb = 0x2F2F2F;
            pP->Old_x = pEm->Cdir_y + 0x800;
            pP->Timer3 = 0;
            pP->Old_z = 100;
            pP->Old_y = 0;
            pP->Timer1 = 30;
            pP->Rot_spd_x = 0;
            pP->Rot_spd_y = pEm->Cdir_y;
            pP->Rot_spd_z = 0x400;
        }
        EM22_W(pEm)->Unk0F |= 1;
    case 1:
        pP = &pEm->pSin_parts_ptr[8];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
        PW_FREE(pP)[1] = -240;
        PW_FREE(pP)[4] = 0;
        PW_FREE(pP)[2] = 0;
        PW_FREE(pP)[3] = 15;
        PW_FREE(pP)[6] = 0;
        for (i = 9; i < 14; i++) {
            pP = &pEm->pSin_parts_ptr[i];
            pP->Be_flg |= 0xA;
            pP->Poly_rgb = 0x2F2F2F;
            pP->Old_x = pEm->Cdir_y + 0x800;
            pP->Timer3 = 0;
            pP->Old_z = 100;
            pP->Old_y = 0;
            pP->Timer1 = 30;
            pP->Rot_spd_x = 0;
            pP->Rot_spd_y = pEm->Cdir_y;
            pP->Rot_spd_z = 0x400;
        }
        EM22_W(pEm)->Unk0F |= 2;
    case 2:
        pP = &pEm->pSin_parts_ptr[15];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
        PW_FREE(pP)[1] = -240;
        PW_FREE(pP)[4] = 0;
        PW_FREE(pP)[2] = 0;
        PW_FREE(pP)[3] = 15;
        PW_FREE(pP)[6] = 0;
        pP = &pEm->pSin_parts_ptr[16];
        pP->Be_flg |= 0xA;
        pP->Poly_rgb = 0x2F2F2F;
        pP->Old_x = pEm->Cdir_y + 0x800;
        pP->Old_z = 100;
        pP->Timer3 = 0;
        pP->Old_y = 0;
        pP->Timer1 = 30;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = pEm->Cdir_y;
        pP->Rot_spd_z = 0x400;
        EM22_W(pEm)->Unk0F |= 4;
    }
    for (i = 0; i < 20; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
        pP->Poly_rgb = 0x202F2F;
    }
}

/* die (id 0x24): the parts given by Unk132 bits 2-3 fall off, then tint all parts 0x4F4F40 */
void func_80108110(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG i;

    EM22_W(pEm)->Unk0F = 0;
    switch ((pEm->Unk132 & 0xC) >> 2) {
    case 0:
        pP = &pEm->pSin_parts_ptr[7];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
        PW_FREE(pP)[1] = -240;
        PW_FREE(pP)[4] = 0;
        PW_FREE(pP)[2] = 0;
        PW_FREE(pP)[3] = 15;
        PW_FREE(pP)[6] = 0;
        EM22_W(pEm)->Unk0F |= 1;
    case 1:
    case 2:
        pP = &pEm->pSin_parts_ptr[16];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
        PW_FREE(pP)[1] = -240;
        PW_FREE(pP)[4] = 0;
        PW_FREE(pP)[2] = 0;
        PW_FREE(pP)[3] = 15;
        PW_FREE(pP)[6] = 0;
    }
    for (i = 0; i < 20; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
        pP->Poly_rgb = 0x4F4F40;
    }
}

/* RE2 Em_dead (routine_0 7): shadow fades in, body twitches now and then */
void func_8010823C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pK = pEm->Kage.pKw;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 40, pEm->Col.Unk39);
        if (EM22_W(pEm)->Unk28 >= 2) {
            if (Joint_move(pEm, pKan, pSeq, 0x100))
                EM22_W(pEm)->Unk28 = 0;
            break;
        }
        pEm->UnkE3 = 30;
        *(ULONG *)&pK->Prim[0].r0 &= 0xFF000000;
        *(ULONG *)&pK->Prim[1].r0 &= 0xFF000000;
        *(ULONG *)&pK->Prim[0].r0 |= 0xBFBF10;
        *(ULONG *)&pK->Prim[1].r0 |= 0xBFBF10;
        pEm->Col.Id = 0;
        EM22_W(pEm)->Unk18 = 0;
        pEm->Routine_2++;
        EM22_W(pEm)->Unk19 = (Rnd_l() & 0x3F) + 30;
    case 1:
        if (--pEm->UnkE3 == 0) {
            pEm->UnkE3 = 120;
            pEm->Routine_2++;
        }
        break;
    case 2:
        pK->Half_x += 5;
        pK->Half_z += 5;
        if (--pEm->UnkE3 == 0)
            pEm->Routine_2++;
        break;
    case 3:
        if (EM22_W(pEm)->Unk28 && ++EM22_W(pEm)->Unk18 > EM22_W(pEm)->Unk19) {
            EM22_W(pEm)->Unk18 = 0;
            EM22_W(pEm)->Unk19 = (Rnd_l() & 0x3F) + 30;
            EM_SET_MOVE(pEm, 0xB, 0, 0xF);
            pEm->Routine_2++;
        }
        break;
    case 4:
        pEm->Routine_2 -= Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    }
}

/* player die routine for id 0x22 (G.pPldie_Em): the player is thrown down */
void func_80108448(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pP;
    LONG i;

    switch (pPl->Routine_2) {
    case 0:
        Pad_vib_set0(10, 10);
        Pad_vib_set0(8, 0x19);
        Pad_vib_set1(20, 200, 0);
        Pad_vib_fade_set(90, 200, 0, 0x14);
        EM_SET_MOVE(pPl, 0, 0, 7);
        pPl->Routine_2 = 1;
        pPl->Timer0 = 0;
        for (i = 0; i < 4; i++) {
            pP = &G.Player.pSin_parts_ptr[Em22_blood_parts[i]];
            PW_FREEL(pP)[0] = 0x20;
            PW_FREEL(pP)[1] = 0x101030;
            pP->UnkA2 |= 4;
        }
    case 1:
        if (pPl->Timer0 == 2) {
            Sv.vx = 0;
            Sv.vy = -400;
            Sv.vz = 0;
            Esp_call(0x200, (USHORT)pPl->Cdir_y | 0x10000000, &pPl->pSin_parts_ptr->Workm, &Sv, 0);
        }
        if (pPl->Timer0 == 8) {
            Sv.vx = 0;
            Sv.vy = -400;
            Sv.vz = 0;
            Esp_call(0x200, (USHORT)pPl->Cdir_y | 0x18000000, &pPl->pSin_parts_ptr->Workm, &Sv, 0);
        }
        if (pPl->Timer0 == 1) {
            pP = &pPl->pSin_parts_ptr[1];
            pP->UnkA2 &= ~0x10;
        }
        pPl->Timer0++;
        Spd_get(pPl, pKan, pSeq, 0);
        pPl->Routine_2 += Joint_move2(pPl, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_SET_RTN(pPl, 7, 0, 0, 0);
        break;
    }
    Add_speedXZ(pPl, 0);
}

/* player damage routine for id 0x24 (G.pPldamage_Em): knocked back, mash to get up */
void func_80108680(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    LONG Spd;
    LONG Next;

    switch (pPl->Routine_2) {
    case 0:
        if (pPl->Routine_3) {
            EM_SET_MOVE(pPl, 3, 0, 7);
            Snd_se_stad(0x10002, POS_V(pPl), 0, 0);
        } else {
            EM_SET_MOVE(pPl, 2, 0, 7);
            Snd_se_stad(0x10001, POS_V(pPl), 0, 0);
        }
        pPl->Timer0 = 200;
        pPl->UnkDA = 0;
        pPl->Routine_2++;
    case 1:
        Spd_get(pPl, pKan, pSeq, 0);
        if (Joint_move2(pPl, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pPl, 1, 0, 0, 0);
            pPl->UnkDA = 1;
            pPl->Damage_cnt &= 0x7F;
        }
        if (Gacha_ch() && !pPl->UnkDA && Joint_move2(pPl, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pPl, 1, 0, 0, 0);
            pPl->Damage_cnt &= 0x7F;
        }
        Spd = pPl->Spd_x;
        if (pPl->Routine_3)
            Next = Spd - pPl->Timer0;
        else
            Next = Spd + pPl->Timer0;
        pPl->Spd_x = Next;
        if (pPl->Timer0)
            pPl->Timer0 -= 50;
        break;
    }
    Add_speedXZ(pPl, 0);
}

/* player die routine for id 0x24 (G.pPldie_Em): the player is torn apart */
void func_8010880C(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEm = pPl->pEnemy_ptr;
    SHORT Dir = (pPl->Routine_3 == 0) << 11;
    SHORT X0;
    SHORT Z0;
    PARTS_W *pP;

    switch (pPl->Routine_2) {
    case 0:
        G.System_flg |= 0x20800;
        if (pPl->Routine_3)
            EM_SET_MOVE(pPl, 1, 0, 7);
        else
            EM_SET_MOVE(pPl, 0, 0, 7);
        pPl->Timer0 = 0;
        pPl->UnkDA = 0;
        pPl->Col.Id = 0;
        pPl->Routine_2++;
    case 1:
        while (pPl->Timer0 == Em22_drop_frame[pPl->UnkDA]) {
            pP = &pPl->pSin_parts_ptr[Em22_drop_parts[pPl->UnkDA]];
            pP->UnkA2 &= ~1;
            pPl->UnkDA++;
        }
        pPl->Timer0++;
        X0 = pPl->Pos_x;
        Z0 = pPl->Pos_z;
        Goto01(pPl, pEm->Cdir_y + Dir, 300);
        pPl->Cdir_y += Dir;
        Null_pos_set(pPl, pKan, pSeq, 0);
        pPl->Cdir_y -= Dir;
        if (Joint_move2(pPl, pKan, pSeq, 0x200))
            pPl->Routine_2++;
        pPl->Pos_x -= (pPl->Pos_x - X0) * pPl->Hokan_flg >> 3;
        pPl->Pos_z -= (pPl->Pos_z - Z0) * pPl->Hokan_flg >> 3;
        break;
    case 2:
        pPl->Timer0 = 0;
        pPl->Routine_2++;
        break;
    case 3:
        if (++pPl->Timer0 >= 31) {
            pPl->Be_flg |= 0x10;
            pPl->Routine_2++;
        }
        break;
    case 4:
        break;
    }
}

/* water splash at the waterline below the body */
void func_80108AAC(ENEMY_WORK *pEm)
{
    SVECTOR Pos;
    SVECTOR Sv;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    CompM((MATRIX *)pEm->Workm_m, PW_M(pP), &pP->Workm);
    Pos.vx = pP->Workm.t[0];
    Pos.vy = pEm->Water;
    Pos.vz = pP->Workm.t[2];
    Sv.vx = 500;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV((MATRIX *)pEm->Workm_m, &Sv, &Sv);
    Sv.vx += Pos.vx;
    Sv.vy += Pos.vy;
    Sv.vz += Pos.vz;
    Esp_call(0x61C, (((Rnd_l() << 2) + 0x1000) << 16) | (USHORT)(pEm->Cdir_y - (Rnd_l() << 2)), &GsIDMATRIX, &Sv, NULL);
    Esp_call(0x61C, (((Rnd_l() << 2) + 0x1000) << 16) | (USHORT)(pEm->Cdir_y + 0x800 - (Rnd_l() << 2)), &GsIDMATRIX, &Pos, NULL);
    Esp_call(0x161C, (((Rnd_l() << 2) + 0xC00) << 16) | (USHORT)(Rnd_l() << 4), &GsIDMATRIX, &Pos, NULL);
    Esp_call(0x161C, (((Rnd_l() << 2) + 0x1000) << 16) | (USHORT)(Rnd_l() << 4), &GsIDMATRIX, &Pos, NULL);
    Sv.vx = 0x400;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x20E, (USHORT)pEm->Cdir_y | 0x28000000, &GsIDMATRIX, &Pos, &Sv);
}

/* is the player in his move routine? */
UCHAR func_80108CA0(void)
{
    return G.Player.Routine_0 == 1;
}

/* no other hunter is already doing routine Rtn */
LONG func_80108CB8(ENEMY_WORK *pEm, ULONG Rtn)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pT;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pT = *ppEm;
        if (pT != pEm && (pT->Id == 0x22 || pT->Id == 0x24) && (pT->Be_flg & 1)
            && (USHORT)EM_RTN01(pT) == (USHORT)Rtn)
            return 0;
    }
    return 1;
}

/* random check, more likely the lower the player's life */
LONG func_80108D44(void)
{
    LONG Lim;

    Lim = 110 - Pl_work.Life;
    return Rnd_l() * 100 / 255 < Lim;
}

/* id 0x24: effect bursts from part 3 driven by the Unk16A sequence (started by a seq flag) */
LONG func_80108DAC(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Unk16A) {
    case 0:
        break;
    case 1:
        pEm->Unk16B = 0;
        pEm->Unk16A++;
        Sv.vx = 0xD0;
        Sv.vy = 0x13D;
        Sv.vz = -0xE2;
        pP = pEm->pSin_parts_ptr;
        pP += 3;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Sv.vx += pP->Workm.t[0];
        Sv.vy += pP->Workm.t[1];
        Sv.vz += pP->Workm.t[2];
        Esp_call(0x39, 0x10000000, &GsIDMATRIX, &Sv, NULL);
        break;
    case 2:
        if (++pEm->Unk16B >= 5) {
            pEm->Unk16B = 0;
            pEm->Unk16A++;
        }
        break;
    case 3:
        pEm->Unk16A++;
        Sv.vx = 0x102;
        Sv.vy = 0x13D;
        Sv.vz = -0xE2;
        pP = pEm->pSin_parts_ptr;
        pP += 3;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Sv.vx += pP->Workm.t[0];
        Sv.vy += pP->Workm.t[1];
        Sv.vz += pP->Workm.t[2];
        Esp_call(0x39, 0x10000000, &GsIDMATRIX, &Sv, NULL);
    case 4:
        if (++pEm->Unk16B >= 4) {
            pEm->Unk16B = 0;
            pEm->Unk16A++;
        }
        break;
    case 5:
        pEm->Unk16A++;
        Sv.vx = 0x148;
        Sv.vy = 0x13D;
        Sv.vz = -0xE2;
        pP = pEm->pSin_parts_ptr;
        pP += 3;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Sv.vx += pP->Workm.t[0];
        Sv.vy += pP->Workm.t[1];
        Sv.vz += pP->Workm.t[2];
        Esp_call(0x39, 0x10000000, &GsIDMATRIX, &Sv, NULL);
    case 6:
        if (++pEm->Unk16B >= 3) {
            pEm->Unk16B = 0;
            pEm->Unk16A++;
        }
        break;
    case 7:
        pEm->Unk16A++;
        Sv.vx = 0x170;
        Sv.vy = 0x13D;
        Sv.vz = -0xE2;
        pP = pEm->pSin_parts_ptr;
        pP += 3;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Sv.vx += pP->Workm.t[0];
        Sv.vy += pP->Workm.t[1];
        Sv.vz += pP->Workm.t[2];
        Esp_call(0x39, 0x10000000, &GsIDMATRIX, &Sv, NULL);
        break;
    case 8:
        pEm->Unk16A++;
        Sv.vx = 0x196;
        Sv.vy = 0x13D;
        Sv.vz = -0xE2;
        pP = pEm->pSin_parts_ptr;
        pP += 3;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Sv.vx += pP->Workm.t[0];
        Sv.vy += pP->Workm.t[1];
        Sv.vz += pP->Workm.t[2];
        Esp_call(0x39, 0x10000000, &GsIDMATRIX, &Sv, NULL);
        break;
    case 9:
        pEm->Unk16A = 0;
        Sv.vx = 0x198;
        Sv.vy = 0x13D;
        Sv.vz = -0xE2;
        pP = pEm->pSin_parts_ptr;
        pP += 3;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Sv.vx += pP->Workm.t[0];
        Sv.vy += pP->Workm.t[1];
        Sv.vz += pP->Workm.t[2];
        Esp_call(0x39, 0x10000000, &GsIDMATRIX, &Sv, NULL);
        break;
    }
}

/* footstep / landing sounds from the seq flags (bits 12..15) */
void func_80109058(ENEMY_WORK *pEm)
{
    VECTOR V;
    USHORT pSeq;
    USHORT No;
    LONG Stad;
    ULONG Code;

    pSeq = *pEm->pNow_seq;
    if (!(pSeq & 0xF000))
        return;
    Stad = 0;
    No = pSeq >> 12;
    if ((pEm->Type & 0xF) == 4)
        Stad = !EM22_W(pEm)->Unk13;
    if (G.Unk783E == 4 && G.Unk7840 == 7 && pEm->Water)
        Stad = 1;
    if (Stad) {
        V.vx = pEm->Pos_x;
        V.vy = pEm->Pos_y;
        V.vz = pEm->Pos_z;
        switch (No - 1) {
        case 0:
            Code = 0x10203;
            break;
        case 1:
            Code = 0x10204;
            break;
        case 2:
        case 11:
            Code = 0x10201;
            break;
        default:
            goto enem;
        }
        Snd_se_stad(Code, &V, 0, 0);
        return;
    }
enem:
    Snd_se_enem(No - 1, pEm);
}

/* is there floor 500 ahead in all four directions? */
LONG func_801091A0(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    VECTOR V;
    LONG Len = 500;

    Sv.vx = Len;
    Sv.vz = 0;
    Rot_vector_l(pEm->Cdir_y, &Sv, &Sv);
    V.vx = pEm->Pos_x + Sv.vx;
    V.vy = pEm->Pos_y;
    V.vz = pEm->Pos_z + Sv.vz;
    if (Sca_get_ground(&V.vx, pEm->Col.W0, 0x200, 0x20))
        return 0;
    Sv.vx = Len;
    Sv.vz = 0;
    Rot_vector_l(pEm->Cdir_y + 0x400, &Sv, &Sv);
    V.vx = pEm->Pos_x + Sv.vx;
    V.vy = pEm->Pos_y;
    V.vz = pEm->Pos_z + Sv.vz;
    if (Sca_get_ground(&V.vx, pEm->Col.W0, 0x200, 0x20))
        return 0;
    Sv.vx = Len;
    Sv.vz = 0;
    Rot_vector_l(pEm->Cdir_y + 0x800, &Sv, &Sv);
    V.vx = pEm->Pos_x + Sv.vx;
    V.vy = pEm->Pos_y;
    V.vz = pEm->Pos_z + Sv.vz;
    if (Sca_get_ground(&V.vx, pEm->Col.W0, 0x200, 0x20))
        return 0;
    Sv.vx = Len;
    Sv.vz = 0;
    Rot_vector_l(pEm->Cdir_y + 0xC00, &Sv, &Sv);
    V.vx = pEm->Pos_x + Sv.vx;
    V.vy = pEm->Pos_y;
    V.vz = pEm->Pos_z + Sv.vz;
    return Sca_get_ground(&V.vx, pEm->Col.W0, 0x200, 0x20) == 0;
}
