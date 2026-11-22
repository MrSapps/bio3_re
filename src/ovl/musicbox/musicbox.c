#include "common.h"
#include "ovl/musicbox.h"
#include "psp.h"
#include "sce1.h"
#include "snd.h"
#include "status.h"

/* main task */
void func_801C2030(void)
{
    tagGLOBAL *pG = &G;
    MBOX_WORK *pMw;
    ULONG i;

    Task_suspend(0);
    pMw = &Mbox_wk;
    pMw->Routine = 0;
    pMw->Mess = 0;
    pMw->Unk2 = 0;
    pMw->Note = 0;
    pMw->Match = 0;
    pMw->Timer = 0;
    pMw->Tune = 1;
    pMw->Run = 1;
    pG->Status_flg |= 0x80;
    func_80038634(0xF00C, 0x60A);
    OM(10).Cdir.vy = 0x3F0;
    for (i = 0; i < MBOX_NOTE_NUM; i++) {
        if (Flg_ck(&pG->Unk7888, i + 6)) {
            OM(i + 11).M.t[0] = Mbox_pos[i * 4];
            OM(i + 11).M.t[2] = Mbox_pos[i * 4 + 1];
        } else {
            OM(i + 11).M.t[0] = Mbox_pos[i * 4 + 2];
            OM(i + 11).M.t[2] = Mbox_pos[i * 4 + 3];
        }
    }
    while (pMw->Run) {
        func_801C2228();
        func_80037C38();
        pG->Unk208C = (ULONG)((ULONG *)pG->pEm_mem + pG->Cbuf_id * 13);
        Om_trans();
        if ((LONG)G.System_flg >= 0)
            Psp_trans();
        if (!pMw->Run)
            break;
        Task_sleep(1);
    }
    G.Status_flg &= ~0x80;
    Task_signal(0);
    Task_exit();
}

/* puzzle state machine */
void func_801C2228(void)
{
    tagGLOBAL *pG = &G;
    MBOX_WORK *pMw = &Mbox_wk;
    USHORT *pCut = pG->pUnk21DC;
    ULONG Se_no;

    switch (pMw->Routine) {
    case 0:
        if (!pMw->Mess) {
            Snd_se_stad(0x700, NULL, 0, 0);
            pMw->Timer = 450;
            pMw->Note = 0;
            pMw->Mess = 1;
            break;
        }
        if (pMw->Timer) {
            if (pMw->Timer % 60 == 0 && pMw->Note < MBOX_NOTE_NUM) {
                Snd_se_stad((pMw->Tune * 6 + 1 + pMw->Note) | 0x200, NULL, 0, 0);
                pMw->Note++;
            }
            if (pMw->Timer > 60 && pMw->Timer <= 430) {
                OM(18).Cdir.vx += 12;
                OM(17).Cdir.vx ^= 0x10;
            }
            pMw->Timer--;
            break;
        }
        pMw->Cut_old = pG->Cut_no;
        pG->Cut_no = pCut[2];
        Main.Unk18E = 1;
        Cut_change(pG->Cut_no);
        pMw->Routine = 1;
        pMw->Mess = 0;
        pMw->Note = 0;
        G.Status_flg |= 0x400000;
        break;
    case 1:
        Mess_set(0xB90022, 0x3400, &Mbox_mess[Mbox_mess_ofs[pMw->Mess]], 0);
        pMw->Routine = 2;
        break;
    case 2:
        if (G.Key_trg & 0x2000) {
            D_800DBB58 = 0;
            if (pMw->Note) {
                pMw->Note--;
                pMw->Routine = 1;
                pMw->Mess = pMw->Note + 1;
                break;
            }
            Snd_se_stad(5, NULL, 0, 0);
            pMw->Run = 0;
            break;
        }
        if (!(D_800DBB58 & 0x80)) {
            if (!pMw->Timer) {
        switch (pG->Unk7848) {
        case 0:
            if (Flg_ck(&pG->Unk7888, pMw->Note))
                pMw->Note_se = 7;
            else
                pMw->Note_se = 1;
            pMw->Note_se += pMw->Note;
            if (!Flg_ck(&pG->Unk7888, pMw->Note + 6)) {
                Flg_on(&pG->Unk7888, pMw->Note + 6);
                OM(pMw->Note + 11).M.t[0] = Mbox_pos[pMw->Note * 4];
                OM(pMw->Note + 11).M.t[2] = Mbox_pos[pMw->Note * 4 + 1];
                Snd_se_stad(0x20E, NULL, 0, 0);
            }
            pMw->Timer = 70;
            pMw->Routine = 3;
            break;
        case 1:
            if (Flg_ck(&pG->Unk7888, pMw->Note))
                pMw->Note_se = 1;
            else
                pMw->Note_se = 7;
            pMw->Note_se += pMw->Note;
            if (Flg_ck(&pG->Unk7888, pMw->Note + 6)) {
                Flg_off(&pG->Unk7888, pMw->Note + 6);
                OM(pMw->Note + 11).M.t[0] = Mbox_pos[pMw->Note * 4 + 2];
                OM(pMw->Note + 11).M.t[2] = Mbox_pos[pMw->Note * 4 + 3];
                Snd_se_stad(0x20E, NULL, 0, 0);
            }
            pMw->Timer = 70;
            pMw->Routine = 3;
            break;
        case 2:
            if (pMw->Note) {
                if (Flg_ck(&pG->Unk7888, pMw->Note) && Flg_ck(&pG->Unk7888, pMw->Note + 6))
                    pMw->Match |= 1 << pMw->Note;
                else if (!Flg_ck(&pG->Unk7888, pMw->Note) && !Flg_ck(&pG->Unk7888, pMw->Note + 6))
                    pMw->Match |= 1 << pMw->Note;
                else
                    pMw->Match &= ~(1 << pMw->Note);
                if (pMw->Note < MBOX_NOTE_NUM - 1) {
                    pMw->Note++;
                    pMw->Routine = 3;
                    break;
                }
                pMw->Routine = 4;
                pMw->Note = 0;
                pMw->Timer = 450;
                Snd_se_stad(0x700, NULL, 0, 0);
                break;
            } else {
                if (Flg_ck(&pG->Unk7888, pMw->Note) && Flg_ck(&pG->Unk7888, pMw->Note + 6))
                    pMw->Match |= 1 << pMw->Note;
                else if (!Flg_ck(&pG->Unk7888, pMw->Note) && !Flg_ck(&pG->Unk7888, pMw->Note + 6))
                    pMw->Match |= 1 << pMw->Note;
                else
                    pMw->Match &= ~(1 << pMw->Note);
                if (pMw->Note < MBOX_NOTE_NUM - 1) {
                    pMw->Note++;
                    pMw->Routine = 3;
                    break;
                }
                pMw->Routine = 4;
                pMw->Note = 0;
                pMw->Timer = 450;
                Snd_se_stad(0x700, NULL, 0, 0);
                break;
            }
        case 3:
            if (pMw->Note) {
                D_800DBB58 = 0;
                pMw->Note--;
                pMw->Routine = 1;
                pMw->Mess = pMw->Note + 1;
                break;
            }
        case 4:
            Snd_se_stad(5, NULL, 0, 0);
            pMw->Run = 0;
            break;
        }
                break;
            }
        } else if (!pMw->Timer) {
            break;
        }
        pMw->Timer--;
        break;
    case 3:
        if (!pMw->Timer) {
            pMw->Routine = 1;
            pMw->Mess = pMw->Note + 1;
            break;
        }
        if (pMw->Timer == 60)
            Snd_se_stad(pMw->Note_se | 0x200, NULL, 0, 0);
        pMw->Timer--;
        break;
    case 4:
        if (pMw->Timer) {
            if (pMw->Timer % 60 == 0 && pMw->Note < MBOX_NOTE_NUM) {
                if ((pMw->Match >> pMw->Note) & 1)
                    Se_no = pMw->Note + 7;
                else
                    Se_no = pMw->Note + 1;
                Snd_se_stad(Se_no | 0x200, NULL, 0, 0);
                pMw->Note++;
            }
            if (pMw->Timer >= 56 && pMw->Timer <= 420) {
                if (pMw->Timer % 2)
                    OM(10).Cdir.vy -= 5;
                else
                    OM(10).Cdir.vy -= 6;
            }
            pMw->Timer--;
            break;
        }
        if (pMw->Tune) {
            if (pMw->Match == 0x3F)
                Flg_on(&pG->Unk7888, 13);
        } else {
            if (!pMw->Match)
                Flg_on(&pG->Unk7888, 13);
        }
        pMw->Run = 0;
        break;
    }
}
