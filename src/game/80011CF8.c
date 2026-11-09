#include "cd.h"
#include "door.h"
#include "em_sub.h"

void Cd_init(void)
{
    ULONG i;
    tagCD *pCd = &CD;

    for (i = CD_RING_BUFFERS; i != 0;) {
        i--;
        pCd->Ring_status[i] = 0;
        pCd->Ring_size[i] = 0;
    }

    while (DsInit() == 0);

    G.Status_flg &= ~STAT_XA_VOICE;
    DsSetDebug(0);
    DsReadyCallback(NULL);
    DsSyncCallback(NULL);

    while (DsCommand(DslSetmode, &Cd_mode, NULL, -1) == 0);

    pCd->Vol = 0x46;
    pCd->Cd_rtn_no = 0;
    pCd->Xa_rtn_no = 0;
    pCd->Use_mode = 0;
    pCd->Unk8C2 = 0xFF;
    pCd->Unk8C3 = 0xFF;
    Set_volume(pCd->Vol);
}

void Cd_system_control(void)
{
    if (CD.Use_mode == 1) {
        Xa_control();
    }
}

INCLUDE_RODATA("asm/nonmatchings/game/80011CF8", D_8001031C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Xa_play);

void Xa_error(void)
{
    func_80074658(0);
    Change_volume(0);
    CD.Xa_rtn_no = 3;
}

void Xa_control(void)
{
    Xa_control_tbl[CD.Xa_rtn_no]();
}

void Xa_control_stop(void)
{
    CD.Use_mode = 0;
    CD.Xa_rtn_no = 0;
    CD.Limit_break = 0;
    G.Status_flg &= ~STAT_XA_VOICE;
}

void Xa_control_init(void)
{
    tagCD *pCd = &CD;

    pCd->Time_ctr = 0;
    do {
        VSync(0);
        if (++pCd->Time_ctr >= 0x3C) {
            CD.Xa_rtn_no = 3;
            return;
        }
    } while (DsQueueLen() != 0);
    CD.Xa_rtn_no = 2;
}

void Xa_seek(UCHAR intr, UCHAR *result)
{
    tagCD *pCd = &CD;
    ULONG i;

    i = 7;
    do {
        pCd->Result[i] = result[i];
    } while (i-- != 0);

    if (intr == DslComplete) {
        pCd->Status++;
        pCd->Limit_break = 0;
        if (DsQueueLen() == 0) {
            DsSyncCallback((DslCB)Xa_ready_cb);
        }
    } else {
        pCd->Status = 0;
    }
}

void Xa_ready_cb(UCHAR intr, UCHAR *result)
{
    tagCD *pCd = &CD;
    ULONG i;

    i = 7;
    do {
        pCd->Result[i] = result[i];
    } while (i-- != 0);

    if (intr == DslComplete) {
        pCd->Status = 1;
        pCd->Limit_break = 0;
        pCd->Result[8] = 0;
        pCd->Current_sector = DsPosToInt((DslLOC *)&pCd->Result[5]);
        if (pCd->End_sector <= pCd->Current_sector) {
            pCd->Xa_rtn_no = 3;
        }
    } else {
        pCd->Status = 0;
    }
}

void Xa_control_play(void)
{
    tagCD *pCd = &CD;

    if (DsSystemStatus() == DslReady) {
        Xa_control_cdmode ^= 1;
        if (Xa_control_cdmode != 0) {
            while (DsCommand(DslGetlocP, NULL, NULL, -1) == 0);
        }
        if (pCd->Status == 0) {
            pCd->Xa_rtn_no = 3;
        }
    }
    if (pCd->Fade_flg != 0) {
        if (pCd->Fade_step < pCd->Vol) {
            pCd->Vol -= pCd->Fade_step;
            Set_volume(pCd->Vol);
        } else {
            pCd->Xa_rtn_no = 3;
        }
    }
    if (pCd->Current_sector < pCd->Start_sector) {
        pCd->Xa_rtn_no = 3;
    }
    pCd->Limit_break++;
    if (pCd->Limit_break > 0x3C) {
        pCd->Xa_rtn_no = 3;
    }
}

void Xa_control_end(void)
{
    CD.Fade_flg = 0;
    CD.Vol = 0x46;
    DsSyncCallback(NULL);

    while (DsCommand(DslPause, NULL, NULL, -1) == 0);

    CD.Xa_rtn_no = 0;
    func_80074658(0);
    Change_volume(0);
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Cd_read_s);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80012818);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80012ECC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800130A0);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80013754);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800138B0);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800139D8);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80013C1C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80013EF8);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80014300);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001480C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80014968);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80014D10);

void Cd_set_read_pos(ULONG size)
{
    tagCD *pCd = &CD;

    pCd->Fsize_read = size;
    pCd->Unk8B8 = size >> 11;
    pCd->Fsector_read = size >> 11;
    pCd->Fsize_now = pCd->Fsize - size;
    pCd->Fsector_num = (pCd->Fsize_now + 0x7FF) >> 11;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800150E4);

void Set_volume(ULONG vol)
{
    tagCD *pCd = &CD;

    if (Stereo_flg == 0) {
        pCd->Cdvol.val0 = vol;
        pCd->Cdvol.val1 = 0;
        pCd->Cdvol.val2 = vol;
        pCd->Cdvol.val3 = 0;
    } else {
        pCd->Cdvol.val0 = vol >> 1;
        pCd->Cdvol.val1 = vol >> 1;
        pCd->Cdvol.val2 = vol >> 1;
        pCd->Cdvol.val3 = vol >> 1;
    }
    DsMix(&pCd->Cdvol);
}

void Change_volume(ULONG mode)
{
    LONG i;

    if (mode == 0) {
        for (i = (LONG)CD.Vol; i >= 0; i--) {
            Set_volume(i);
        }
    } else {
        i = 0;
        do {
            Set_volume(i);
            i++;
        } while ((ULONG)i <= CD.Vol);
    }
}

void Xa_fade_set(ULONG frames)
{
    tagCD *pCd = &CD;

    pCd->Fade_flg = 1;
    pCd->Fade_step = pCd->Vol / ((UCHAR)frames - 1);
}

void Cd_seek(ULONG File_no, ULONG Mode)
{
    tagCD *pCd = &CD;

    if (Mode < 2) {
        pCd->Fsum = floc[File_no].sum;
        pCd->Fsize = floc[File_no].size;
        pCd->Fsector = (ULONG)floc[File_no].sec_high * 0x10000 + floc[File_no].sector;
    }
    pCd->Fno = File_no;
    pCd->Current_sector = pCd->Fsector;
    DsFlush();
    DsIntToPos(pCd->Current_sector, &pCd->Fpos);
    while (DsCommand(DslPause, NULL, NULL, -1) == 0);
    while (DsCommand(DslSeekL, (u_char *)&pCd->Fpos, NULL, -1) == 0);
}

void Door_main(void)
{
    UCHAR i;

    for (i = 0; Door_proc_tbl[i] != NULL; i++) {
        Door_proc_tbl[i]();
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_init);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_move);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_scheduler_main);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80015964);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80015D98);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_exit);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016208);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_model_init);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_model_set);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_snd_trans);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016600);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800167AC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800167E4);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001681C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016854);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001688C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016A40);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016BF4);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016C9C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016DA0);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016E80);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016F44);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80017008);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80017148);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800171DC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800172AC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80017318);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80017540);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80017574);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001760C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80017ADC);

void func_80017D9C(void) {
}

ULONG Hani_ck(VECTOR *pV, SHORT *pPxzhw)
{
    if ((ULONG)(pV->vx - pPxzhw[0]) > (ULONG)pPxzhw[2] || (ULONG)(pV->vz - pPxzhw[1]) > (ULONG)pPxzhw[3]) {
        return 0;
    }
    return 1;
}

void Add_speedXZ(ENEMY_WORK *pEm, LONG muki)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    if ((pEm->UnkAE & 0x40) && !(pEm->Be_flg & BE_FLG_400)) {
        pEm->Spd_x = (SHORT)(pEm->Spd_x * 3) >> 2;
    }
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_z += Sv.vz;
}

void Add_speedXYZ(ENEMY_WORK *pEm, LONG muki_y, LONG muki_z)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki_y;
    Sv.vz = pEm->Cdir_z + (SHORT)muki_z;
    RotMatrix_gte(&Sv, &M);
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_y += Sv.vy;
    pEm->Pos_z += Sv.vz;
}

void Get_speedXZ(ENEMY_WORK *pEm, LONG muki, SHORT *pX, SHORT *pZ)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
    *pX = Sv.vx;
    *pZ = Sv.vz;
}

void Add_speedXYZsuper(ENEMY_WORK *pEm, LONG muki_y, LONG muki_z)
{
    SVECTOR Sv;
    MATRIX M;
    MATRIX Tm;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki_y;
    Sv.vz = pEm->Cdir_z + (SHORT)muki_z;
    RotMatrix_gte(&Sv, &M);
    func_8002D4B0(pEm->pSuper, &M, &Tm);
    ApplyMatrixSV(&Tm, (SVECTOR *)&pEm->Spd_x, &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_y += Sv.vy;
    pEm->Pos_z += Sv.vz;
}

LONG Direction_ck(LONG Hontai_x, LONG Hontai_z, LONG Target_x, LONG Target_z)
{
    USHORT Dir;
    LONG ZZ;
    LONG XX;
    LONG Dz;

    Dir = 0x400;
    XX = (SHORT)Target_x - (SHORT)Hontai_x;
    Dz = (SHORT)Target_z - (SHORT)Hontai_z;
    if (Dz > 0) {
        Dir = 0xC00;
    }
    if (XX != 0) {
        ZZ = (SHORT)catan(Dz * 0x1000 / XX);
        Dir = ((XX < 0 ? 0x800 : 0x1000) - ZZ) & 0xFFF;
    }
    return Dir;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Goto00);

LONG Goto00_ck(ENEMY_WORK *pEm, LONG Vec_x, LONG Vec_z, LONG Add_dir)
{
    ULONG Ang_err;

    Ang_err = (Add_dir + (Direction_ck((SHORT)pEm->Pos_x, (SHORT)pEm->Pos_z, (SHORT)Vec_x, (SHORT)Vec_z) - (USHORT)pEm->Cdir_y)) & 0xFFF;
    if ((LONG)Ang_err < (SHORT)Add_dir * 2) {
        return 0;
    }
    if (Ang_err < 0x801) {
        return (SHORT)Add_dir;
    }
    return -(SHORT)Add_dir;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Goto01);

LONG Goto01_ck(ENEMY_WORK *pEm, LONG Dir, LONG Add_dir)
{
    ULONG Rot;

    Rot = (Add_dir + (Dir - (USHORT)pEm->Cdir_y)) & 0xFFF;
    if ((LONG)Rot < (SHORT)Add_dir * 2) {
        return 0;
    }
    if (Rot < 0x801) {
        return (SHORT)Add_dir;
    }
    return -(SHORT)Add_dir;
}

LONG Dir_pos_ck(VECTOR *pV, VECTOR *pP, SHORT Hed_dir, SHORT Hani_dir)
{
    SHORT Dir;

    Dir = Direction_ck((SHORT)pV->vx, (SHORT)pV->vz, (SHORT)pP->vx, (SHORT)pP->vz);
    return ((((LONG)Dir - (LONG)Hed_dir) + (LONG)Hani_dir) & 0xFFFU) < (ULONG)((LONG)Hani_dir << 1) ^ 1;
}

LONG L_pos_ckXZ(VECTOR *pV, VECTOR *pP, ULONG Ll)
{
    LONG Dx;
    LONG Dz;

    Dx = (pV->vx - pP->vx) >> 1;
    Dz = (pV->vz - pP->vz) >> 1;
    return (ULONG)(SquareRoot0(Dx * Dx + Dz * Dz) * 2) < Ll;
}

LONG L_pos_ckXZM(MATRIX *pM, VECTOR *pP, ULONG Ll, SVECTOR *pSv)
{
    LONG Dx;
    LONG Dz;
    SVECTOR Sv;
    VECTOR V;

    if (pSv == NULL) {
        *(ULONG *)&Sv.vx = 0;
        Sv.vz = 0;
    } else {
        *(ULONG *)&Sv.vx = *(ULONG *)&pSv->vx;
        Sv.vz = pSv->vz;
    }
    ApplyMatrix(pM, &Sv, &V);
    V.vx += pM->t[0];
    V.vy += pM->t[1];
    V.vz += pM->t[2];
    Dx = (V.vx - pP->vx) >> 1;
    Dz = (V.vz - pP->vz) >> 1;
    return (ULONG)(SquareRoot0(Dx * Dx + Dz * Dz) * 2) < Ll;
}

UCHAR Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    return ((((LONG)pT->Cdir_y - (LONG)pEm->Cdir_y) + 0x400) & 0xFFF) < 0x800;
}

UCHAR Cdir_ck_pos(LONG Pos_x, LONG Pos_z, ENEMY_WORK *pT)
{
    return (ULONG)(((USHORT)pT->Cdir_y - Direction_ck((SHORT)Pos_x, (SHORT)Pos_z, (SHORT)pT->Pos_x, (SHORT)pT->Pos_z)) + 0x400 & 0xFFF) < 0x800;
}

UCHAR Cdir_ck2(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SHORT Dir;

    Dir = Direction_ck((SHORT)pEm->Pos_x, (SHORT)pEm->Pos_z, (SHORT)pT->Pos_x, (SHORT)pT->Pos_z);
    return ((((LONG)pT->Cdir_y - (LONG)Dir) + 0x400) & 0xFFF) < 0x800;
}

UCHAR Dir_ck(LONG Dir0, LONG Dir1)
{
    return ((((SHORT)Dir1 - (SHORT)Dir0) + 0x400) & 0xFFF) < 0x800;
}

ULONG Gacha_ch(void)
{
    return (G.Key_trg & 0xF4F) != 0;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800185D0);

void Set_base_pos(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;
    ENEMY_WORK *pPm;

    pPm = G.pEm;
    get_null_pos(&sv, R_flg, Kan, Seq);
    m = GsIDMATRIX;
    RotMatrixY(pPm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pPm->Base_pos_x = (SHORT)pPm->Pos_x - sv.vx;
    pPm->Base_pos_y = (SHORT)pPm->Pos_y - sv.vy;
    pPm->Base_pos_z = (SHORT)pPm->Pos_z - sv.vz;
    pEm->Base_pos_x = pPm->Base_pos_x;
    pEm->Base_pos_y = pPm->Base_pos_y;
    pEm->Base_pos_z = pPm->Base_pos_z;
}

void Null_pos_set(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;

    get_null_pos(&sv, R_flg, Kan, Seq);
    m = GsIDMATRIX;
    RotMatrixY(pEm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pEm->Pos_x = pEm->Base_pos_x + sv.vx;
    pEm->Pos_y = pEm->Base_pos_y + sv.vy;
    pEm->Pos_z = pEm->Base_pos_z + sv.vz;
}

void get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pEm;
    USHORT *pSeq;
    ULONG f_num;
    ULONG f_base;
    ULONG Rec_sz;
    UCHAR *Bone_adr;

    pEm = G.pEm;
    pSeq = (USHORT *)(Seq + pEm->Move_no * 8);
    f_num = pSeq[0];
    f_base = pSeq[2];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)(Seq + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)(Seq + *pSeq) + pEm->Move_cnt;
    }
    Bone_adr = (UCHAR *)Kan + ((KAN_HEAD *)Kan)->Bone_ofs;
    Rec_sz = ((KAN_HEAD *)Kan)->Rec_sz >> 18;
    while (*pSeq & 0x100) {
        pSeq++;
    }
    Bone_adr += Rec_sz * (f_base + *(UCHAR *)pSeq) * 4;
    pSv->vx = ((SVECTOR *)Bone_adr)->vx;
    pSv->vy = ((SVECTOR *)Bone_adr)->vy;
    pSv->vz = ((SVECTOR *)Bone_adr)->vz;
}

void Null_pos_set2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;

    get_null_pos2(&sv, R_flg, Kan, Seq);
    m = GsIDMATRIX;
    RotMatrixY(pEm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pEm->Pos_x = pEm->Base_pos_x + sv.vx;
    pEm->Pos_y = pEm->Base_pos_y + sv.vy;
    pEm->Pos_z = pEm->Base_pos_z + sv.vz;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", get_null_pos2);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Spd_get);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80018EC8);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019178);

void Em_seq_sound(ENEMY_WORK *pEm)
{
    if (*pEm->pNow_seq & 0xF000) {
        func_800775A0((*pEm->pNow_seq >> 12) - 1, pEm);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019318);

void Kage_work_init(void)
{
    KAGE_WORK *pK;
    ULONG i;
    POLY_FT4 *p;

    pK = G.Kage;
    i = 0x32;
    do {
        p = &pK->Prim[0];
        pK->Be_flg = 0;
        *(ULONG *)&p->r0 = 0x808080;
        SetPolyFT4(p);
        SetSemiTrans(p, 1);
        p->tpage = GetTPage(1, 2, 0x240, 0x100);
        p->clut = GetClut(0, 0x1E3);
        p->u0 = 0x62;
        p->v0 = 0xE1;
        p->u1 = 0x7E;
        p->v1 = 0xE1;
        p->u2 = 0x62;
        p->v2 = 0xFF;
        p->u3 = 0x7E;
        p->v3 = 0xFF;
        func_800100A4(&pK->Prim[1], p, 0x28);
        pK++;
        i--;
    } while (i != 0);
}

void Kage_work9_init(void)
{
    KAGE_WORK9 *pK;
    ULONG i;

    pK = G.Kage9;
    i = 5;
    do {
        pK->Be_flg = 0;
        i--;
        pK++;
    } while (i != 0);
}

void Kage_work_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos)
{
    KAGE_WORK *pKw;
    LONG Left_cnt;

    pKw = G.Kage;
    Left_cnt = 0x32;
    while (pKw->Be_flg != 0) {
        Left_cnt--;
        if (Left_cnt == 0) {
            break;
        }
        pKw++;
    }
    if (Left_cnt == 0) {
        *pK = 0;
    } else {
        *pK = (ULONG)pKw;
        pKw->Be_flg = 5;
        pKw->pPos = pPos;
        if (Color != 0) {
            *(ULONG *)&pKw->Prim[0].r0 = (*(ULONG *)&pKw->Prim[0].r0 & 0xFF000000) | Color;
            *(ULONG *)&pKw->Prim[1].r0 = (*(ULONG *)&pKw->Prim[1].r0 & 0xFF000000) | Color;
        } else {
            *(ULONG *)&pKw->Prim[0].r0 = (*(ULONG *)&pKw->Prim[0].r0 & 0xFF000000) | 0x808080;
            *(ULONG *)&pKw->Prim[1].r0 = (*(ULONG *)&pKw->Prim[1].r0 & 0xFF000000) | 0x808080;
        }
        *(ULONG *)&pKw->Off_x = Off;
        *(ULONG *)&pKw->Half_x = Half;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Kage_work9_set);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019A50);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019B3C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019DF4);

void Kage_work_color_set(ULONG *pK, ULONG Color)
{
    *(ULONG *)&((KAGE_WORK *)pK)->Prim[0].r0 = (*(ULONG *)&((KAGE_WORK *)pK)->Prim[0].r0 & 0xFF000000) | Color;
    *(ULONG *)&((KAGE_WORK *)pK)->Prim[1].r0 = (*(ULONG *)&((KAGE_WORK *)pK)->Prim[1].r0 & 0xFF000000) | Color;
}

void Kage_work9_color_set(ULONG *pK, ULONG Color)
{
    KAGE_WORK *pPrim;
    UCHAR cnt;

    cnt = 9;
    pPrim = ((KAGE_WORK9 *)pK)->pKw_top;
    do {
        cnt--;
        *(ULONG *)&pPrim->Prim[0].r0 = (*(ULONG *)&pPrim->Prim[0].r0 & 0xFF000000) | Color;
        *(ULONG *)&pPrim->Prim[1].r0 = (*(ULONG *)&pPrim->Prim[1].r0 & 0xFF000000) | Color;
        pPrim++;
    } while (cnt != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001A248);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001A808);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AA3C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AC58);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AD78);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AE2C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AECC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AF3C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B048);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B08C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B148);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B204);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B2A4);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B35C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B484);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B894);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BA48);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BB24);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BC80);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BCFC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BFCC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C168);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C254);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C2DC);

void func_8001C384(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C38C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C46C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C49C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C5BC);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001C98C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001CB04);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001CC60);

void func_8001CD64(void) {
}

void func_8001CD6C(void) {
}

void func_8001CD74(void) {
}

void func_8001CD7C(void) {
}

void func_8001CD84(void) {
}

void func_8001CD8C(void) {
}

void func_8001CD94(void) {
}

void func_8001CD9C(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001CDA4);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001CE24);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001CF24);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001CFF4);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001D210);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001D480);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001D534);

void func_8001D7C0(void) {
}

void func_8001D7C8(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001D7D0);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001E0F4);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001E444);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001F208);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001F378);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001F4A8);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001F4F4);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001F674);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001F824);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001FA90);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001FDD0);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001FF6C);
