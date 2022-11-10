#include <buildins.h>
#include <string.h>
#include <stdio.h>

union any and_cc(union any v1, union any v2) { return (union any) {.b = v1.c && v2.c}; }
union any and_ci(union any v1, union any v2) { return (union any) {.b = v1.c && v2.i}; }
union any and_cf(union any v1, union any v2) { return (union any) {.b = v1.c && v2.f}; }
union any and_cs(union any v1, union any v2) { return (union any) {.b = v1.c && v2.s.len}; }
union any and_cp(union any v1, union any v2) { return (union any) {.b = v1.c && v2.p}; }
union any and_cb(union any v1, union any v2) { return (union any) {.b = v1.c && v2.b}; }

union any and_ic(union any v1, union any v2) { return (union any) {.b = v1.i && v2.c}; }
union any and_ii(union any v1, union any v2) { return (union any) {.b = v1.i && v2.i}; }
union any and_if(union any v1, union any v2) { return (union any) {.b = v1.i && v2.f}; }
union any and_is(union any v1, union any v2) { return (union any) {.b = v1.i && v2.s.len}; }
union any and_ip(union any v1, union any v2) { return (union any) {.b = v1.i && v2.p}; }
union any and_ib(union any v1, union any v2) { return (union any) {.b = v1.i && v2.b}; }

union any and_fc(union any v1, union any v2) { return (union any) {.b = v1.f && v2.c}; }
union any and_fi(union any v1, union any v2) { return (union any) {.b = v1.f && v2.i}; }
union any and_ff(union any v1, union any v2) { return (union any) {.b = v1.f && v2.f}; }
union any and_fs(union any v1, union any v2) { return (union any) {.b = v1.f && v2.s.len}; }
union any and_fp(union any v1, union any v2) { return (union any) {.b = v1.f && v2.p}; }
union any and_fb(union any v1, union any v2) { return (union any) {.b = v1.f && v2.b}; }

union any and_sc(union any v1, union any v2) { return (union any) {.b = v1.s.len && v2.c}; }
union any and_si(union any v1, union any v2) { return (union any) {.b = v1.s.len && v2.i}; }
union any and_sf(union any v1, union any v2) { return (union any) {.b = v1.s.len && v2.f}; }
union any and_ss(union any v1, union any v2) { return (union any) {.b = v1.s.len && v2.s.len}; }
union any and_sp(union any v1, union any v2) { return (union any) {.b = v1.s.len && v2.p}; }
union any and_sb(union any v1, union any v2) { return (union any) {.b = v1.s.len && v2.b}; }

union any and_pc(union any v1, union any v2) { return (union any) {.b = v1.p && v2.c}; }
union any and_pi(union any v1, union any v2) { return (union any) {.b = v1.p && v2.i}; }
union any and_pf(union any v1, union any v2) { return (union any) {.b = v1.p && v2.f}; }
union any and_ps(union any v1, union any v2) { return (union any) {.b = v1.p && v2.s.len}; }
union any and_pp(union any v1, union any v2) { return (union any) {.b = v1.p && v2.p}; }
union any and_pb(union any v1, union any v2) { return (union any) {.b = v1.p && v2.b}; }

union any and_bc(union any v1, union any v2) { return (union any) {.b = v1.b && v2.c}; }
union any and_bi(union any v1, union any v2) { return (union any) {.b = v1.b && v2.i}; }
union any and_bf(union any v1, union any v2) { return (union any) {.b = v1.b && v2.f}; }
union any and_bs(union any v1, union any v2) { return (union any) {.b = v1.b && v2.s.len}; }
union any and_bp(union any v1, union any v2) { return (union any) {.b = v1.b && v2.p}; }
union any and_bb(union any v1, union any v2) { return (union any) {.b = v1.b && v2.b}; }

// OR FUNCTION BUILDINS
union any or_cc(union any v1, union any v2) { return (union any) {.b = v1.c || v2.c}; }
union any or_ci(union any v1, union any v2) { return (union any) {.b = v1.c || v2.i}; }
union any or_cf(union any v1, union any v2) { return (union any) {.b = v1.c || v2.f}; }
union any or_cs(union any v1, union any v2) { return (union any) {.b = v1.c || v2.s.len}; }
union any or_cp(union any v1, union any v2) { return (union any) {.b = v1.c || v2.p}; }
union any or_cb(union any v1, union any v2) { return (union any) {.b = v1.c || v2.b}; }

union any or_ic(union any v1, union any v2) { return (union any) {.b = v1.i || v2.c}; }
union any or_ii(union any v1, union any v2) { return (union any) {.b = v1.i || v2.i}; }
union any or_if(union any v1, union any v2) { return (union any) {.b = v1.i || v2.f}; }
union any or_is(union any v1, union any v2) { return (union any) {.b = v1.i || v2.s.len}; }
union any or_ip(union any v1, union any v2) { return (union any) {.b = v1.i || v2.p}; }
union any or_ib(union any v1, union any v2) { return (union any) {.b = v1.i || v2.b}; }

union any or_fc(union any v1, union any v2) { return (union any) {.b = v1.f || v2.c}; }
union any or_fi(union any v1, union any v2) { return (union any) {.b = v1.f || v2.i}; }
union any or_ff(union any v1, union any v2) { return (union any) {.b = v1.f || v2.f}; }
union any or_fs(union any v1, union any v2) { return (union any) {.b = v1.f || v2.s.len}; }
union any or_fp(union any v1, union any v2) { return (union any) {.b = v1.f || v2.p}; }
union any or_fb(union any v1, union any v2) { return (union any) {.b = v1.f || v2.b}; }

union any or_sc(union any v1, union any v2) { return (union any) {.b = v1.s.len || v2.c}; }
union any or_si(union any v1, union any v2) { return (union any) {.b = v1.s.len || v2.i}; }
union any or_sf(union any v1, union any v2) { return (union any) {.b = v1.s.len || v2.f}; }
union any or_ss(union any v1, union any v2) { return (union any) {.b = v1.s.len || v2.s.len}; }
union any or_sp(union any v1, union any v2) { return (union any) {.b = v1.s.len || v2.p}; }
union any or_sb(union any v1, union any v2) { return (union any) {.b = v1.s.len || v2.b}; }

union any or_pc(union any v1, union any v2) { return (union any) {.b = v1.p || v2.c}; }
union any or_pi(union any v1, union any v2) { return (union any) {.b = v1.p || v2.i}; }
union any or_pf(union any v1, union any v2) { return (union any) {.b = v1.p || v2.f}; }
union any or_ps(union any v1, union any v2) { return (union any) {.b = v1.p || v2.s.len}; }
union any or_pp(union any v1, union any v2) { return (union any) {.b = v1.p || v2.p}; }
union any or_pb(union any v1, union any v2) { return (union any) {.b = v1.p || v2.b}; }

union any or_bc(union any v1, union any v2) { return (union any) {.b = v1.b || v2.c}; }
union any or_bi(union any v1, union any v2) { return (union any) {.b = v1.b || v2.i}; }
union any or_bf(union any v1, union any v2) { return (union any) {.b = v1.b || v2.f}; }
union any or_bs(union any v1, union any v2) { return (union any) {.b = v1.b || v2.s.len}; }
union any or_bp(union any v1, union any v2) { return (union any) {.b = v1.b || v2.p}; }
union any or_bb(union any v1, union any v2) { return (union any) {.b = v1.b || v2.b}; }

/* ADDITION FUNCTION BUILDINS
add_cc, add_ci, add_cs, add_ic, add_ii, add_if, add_is,
add_ip, add_fi, add_ff, add_fs, add_pi, add_ps, add_sc,
add_si, add_sf, add_ss, add_sp, add_sb, add_bc, add_bi,
add_bf, add_bs, add_bp
*/ 
string_t strconc(const char* v1, size_t l1, const char* v2, size_t l2) {
    char* out = malloc(l1 + l2 + 1);
    memcpy(out, v1, l1);
    memcpy(out + l1, v2, l2);
    out[l1 + l2] = 0;
    return (string_t) { out, l1 + l2 };
}

union any add_cc(union any v1, union any v2) { return (union any) {.s = strconc(&v1.c, 1, &v2.c, 1)}; }
union any add_ci(union any v1, union any v2) { return (union any) {.i = v1.c + v2.i}; }
union any add_cs(union any v1, union any v2) { return (union any) {.s = strconc(&v1.c, 1, v2.s.val, v2.s.len)}; }

union any add_ic(union any v1, union any v2) { return (union any) {.i = v1.i + v2.c}; }
union any add_ii(union any v1, union any v2) { return (union any) {.i = v1.i + v2.i}; }
union any add_if(union any v1, union any v2) { return (union any) {.f = v1.i + v2.f}; }
union any add_is(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%ld", v1.i);
    return (union any) {.s = strconc(ibuf, len, v2.s.val, v2.s.len)};
}

union any add_fi(union any v1, union any v2) { return (union any) {.b = v1.f + v2.i}; }
union any add_ff(union any v1, union any v2) { return (union any) {.b = v1.f + v2.f}; }
union any add_fs(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%f", v1.f);
    return (union any) {.s = strconc(ibuf, len, v2.s.val, v2.s.len)};
}

union any add_ps(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%p", v1.p);
    return (union any) {.s = strconc(ibuf, len, v2.s.val, v2.s.len)};
}

union any add_sc(union any v1, union any v2) { return (union any) {.s = strconc(v1.s.val, v1.s.len, &v2.c, 1)}; }
union any add_si(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%ld", v2.i);
    return (union any) {.s = strconc(v1.s.val, v1.s.len, ibuf, len)};
}
union any add_sf(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%f", v2.f);
    return (union any) {.s = strconc(v1.s.val, v1.s.len, ibuf, len)};
}
union any add_ss(union any v1, union any v2) {
    return (union any) {.s = strconc(v1.s.val, v1.s.len, v2.s.val, v2.s.len)};
}
union any add_sp(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%p", v2.p);
    return (union any) {.s = strconc(v1.s.val, v1.s.len, ibuf, len)};
}
union any add_sb(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%s", v2.b ? "true" : "false");
    return (union any) {.s = strconc(v1.s.val, v1.s.len, ibuf, len)};
}

union any add_bs(union any v1, union any v2) {
    char ibuf[64];
    size_t len = snprintf(ibuf, 64, "%s", v1.b ? "true" : "false");
    return (union any) {.s = strconc(ibuf, len, v2.s.val, v2.s.len)};
}

union any sub_ci(union any v1, union any v2) { return (union any) {.i = v1.c - v2.i}; }

union any sub_ic(union any v1, union any v2) { return (union any) {.c = v1.i - v2.c}; }
union any sub_ii(union any v1, union any v2) { return (union any) {.i = v1.i - v2.i}; }
union any sub_if(union any v1, union any v2) { return (union any) {.f = v1.i - v2.f}; }

union any sub_fi(union any v1, union any v2) { return (union any) {.b = v1.f - v2.i}; }
union any sub_ff(union any v1, union any v2) { return (union any) {.b = v1.f - v2.f}; }

struct op_func op_functions[16][7][7] = {
    [AND] {
        [EVAL_UNSPECIFIED] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_CHAR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      and_cc},
            [EVAL_INT]          {EVAL_BOOLEAN,      and_ci},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      and_cf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      and_cs},
            [EVAL_PTR]          {EVAL_BOOLEAN,      and_cp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      and_cb},
        },
        [EVAL_INT] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      and_ic},
            [EVAL_INT]          {EVAL_BOOLEAN,      and_ii},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      and_if},
            [EVAL_STRING]       {EVAL_BOOLEAN,      and_is},
            [EVAL_PTR]          {EVAL_BOOLEAN,      and_ip},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      and_ib},
        },
        [EVAL_FLOAT]        {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      and_fc},
            [EVAL_INT]          {EVAL_BOOLEAN,      and_fi},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      and_ff},
            [EVAL_STRING]       {EVAL_BOOLEAN,      and_fs},
            [EVAL_PTR]          {EVAL_BOOLEAN,      and_fp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      and_fb},
        },
        [EVAL_PTR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      and_pc},
            [EVAL_INT]          {EVAL_BOOLEAN,      and_pi},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      and_pf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      and_ps},
            [EVAL_PTR]          {EVAL_BOOLEAN,      and_pp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      and_pb},
        },
        [EVAL_STRING] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      and_sc},
            [EVAL_INT]          {EVAL_BOOLEAN,      and_si},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      and_sf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      and_ss},
            [EVAL_PTR]          {EVAL_BOOLEAN,      and_sp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      and_sb},
        },
        [EVAL_BOOLEAN] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      and_bc},
            [EVAL_INT]          {EVAL_BOOLEAN,      and_bi},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      and_bf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      and_bs},
            [EVAL_PTR]          {EVAL_BOOLEAN,      and_bp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      and_bb},
        },
    },
    [OR] {
        [EVAL_UNSPECIFIED] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_CHAR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      or_cc},
            [EVAL_INT]          {EVAL_BOOLEAN,      or_ci},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      or_cf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      or_cs},
            [EVAL_PTR]          {EVAL_BOOLEAN,      or_cp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      or_cb},
        },
        [EVAL_INT] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      or_ic},
            [EVAL_INT]          {EVAL_BOOLEAN,      or_ii},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      or_if},
            [EVAL_STRING]       {EVAL_BOOLEAN,      or_is},
            [EVAL_PTR]          {EVAL_BOOLEAN,      or_ip},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      or_ib},
        },
        [EVAL_FLOAT]        {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      or_fc},
            [EVAL_INT]          {EVAL_BOOLEAN,      or_fi},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      or_ff},
            [EVAL_STRING]       {EVAL_BOOLEAN,      or_fs},
            [EVAL_PTR]          {EVAL_BOOLEAN,      or_fp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      or_fb},
        },
        [EVAL_PTR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      or_pc},
            [EVAL_INT]          {EVAL_BOOLEAN,      or_pi},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      or_pf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      or_ps},
            [EVAL_PTR]          {EVAL_BOOLEAN,      or_pp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      or_pb},
        },
        [EVAL_STRING] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      or_sc},
            [EVAL_INT]          {EVAL_BOOLEAN,      or_si},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      or_sf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      or_ss},
            [EVAL_PTR]          {EVAL_BOOLEAN,      or_sp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      or_sb},
        },
        [EVAL_BOOLEAN] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_BOOLEAN,      or_bc},
            [EVAL_INT]          {EVAL_BOOLEAN,      or_bi},
            [EVAL_FLOAT]        {EVAL_BOOLEAN,      or_bf},
            [EVAL_STRING]       {EVAL_BOOLEAN,      or_bs},
            [EVAL_PTR]          {EVAL_BOOLEAN,      or_bp},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      or_bb},
        },
    },
    [ADD] {
        [EVAL_UNSPECIFIED] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_CHAR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_STRING,       add_cc},
            [EVAL_INT]          {EVAL_INT,          add_ci},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_STRING,       add_cs},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_INT] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_INT,          add_ic},
            [EVAL_INT]          {EVAL_INT,          add_ii},
            [EVAL_FLOAT]        {EVAL_FLOAT,        add_if},
            [EVAL_STRING]       {EVAL_STRING,       add_is},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_FLOAT]        {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_FLOAT,        add_fi},
            [EVAL_FLOAT]        {EVAL_FLOAT,        add_ff},
            [EVAL_STRING]       {EVAL_STRING,       add_fs},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_PTR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  add_ps},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_STRING] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_STRING,       add_sc},
            [EVAL_INT]          {EVAL_STRING,       add_si},
            [EVAL_FLOAT]        {EVAL_STRING,       add_sf},
            [EVAL_STRING]       {EVAL_STRING,       add_ss},
            [EVAL_PTR]          {EVAL_STRING,       add_sp},
            [EVAL_BOOLEAN]      {EVAL_STRING,       add_sb},
        },
        [EVAL_BOOLEAN] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_STRING,       add_bs},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_BOOLEAN,      or_bb},
        },
    },
    [SUBTRACT] {
        [EVAL_UNSPECIFIED] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_CHAR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_INT,          sub_ci},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_INT] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_INT,          sub_ic},
            [EVAL_INT]          {EVAL_INT,          sub_ii},
            [EVAL_FLOAT]        {EVAL_FLOAT,        sub_if},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_FLOAT]        {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_FLOAT,        sub_fi},
            [EVAL_FLOAT]        {EVAL_FLOAT,        sub_ff},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_PTR] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_STRING] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
        [EVAL_BOOLEAN] {
            [EVAL_UNSPECIFIED]  {EVAL_UNSPECIFIED,  NULL},
            [EVAL_CHAR]         {EVAL_UNSPECIFIED,  NULL},
            [EVAL_INT]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_FLOAT]        {EVAL_UNSPECIFIED,  NULL},
            [EVAL_STRING]       {EVAL_UNSPECIFIED,  NULL},
            [EVAL_PTR]          {EVAL_UNSPECIFIED,  NULL},
            [EVAL_BOOLEAN]      {EVAL_UNSPECIFIED,  NULL},
        },
    }
};