/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * rflags.c - Exports a Python class, called `Rflags', that represents the CPU
 * flags register contents.
 */
#include "includes.h"
#include "rflags.h"


/* CPU flags along with a brief description. */
static PyMemberDef members[] =
{
    {"_if", T_UINT, offsetof(rflags_t, _if), 0, "Interrupt enable flag"},
    {"ac", T_UINT, offsetof(rflags_t, ac), 0, "Alignment check"},
    {"af", T_UINT, offsetof(rflags_t, af), 0, "Adjust flag"},
    {"cf", T_UINT, offsetof(rflags_t, cf), 0, "Carry flag"},
    {"df", T_UINT, offsetof(rflags_t, df), 0, "Direction flag"},
    {"id", T_UINT, offsetof(rflags_t, id), 0, "Identification flag"},
    {"nt", T_UINT, offsetof(rflags_t, nt), 0, "Nested task flag"},
    {"of", T_UINT, offsetof(rflags_t, of), 0, "Overflow flag"},
    {"pf", T_UINT, offsetof(rflags_t, pf), 0, "Parity flag"},
    {"rf", T_UINT, offsetof(rflags_t, rf), 0, "Resume flag"},
    {"sf", T_UINT, offsetof(rflags_t, sf), 0, "Sign flag"},
    {"tf", T_UINT, offsetof(rflags_t, tf), 0, "Trap flag"},
    {"vif", T_UINT, offsetof(rflags_t, vif), 0, "Virtual interrupt flag"},
    {"vip", T_UINT, offsetof(rflags_t, vip), 0, "Virtual interrupt pending"},
    {"vm", T_UINT, offsetof(rflags_t, vm), 0, "Virtual 8086 mode flag"},
    {"zf", T_UINT, offsetof(rflags_t, zf), 0, "Zero flag"},
    {NULL, 0, 0, 0, NULL}
};


/* See comment in "decoder.c" for more information. */
static PyObject type_base =
{
    PyObject_HEAD_INIT(NULL)
};

static PyTypeObject type;


/* Allocate and initialize a new `Rflags' object given a `xed_flag_set_t'. */
rflags_t *new_rflags(const xed_flag_set_t *flags)
{
    rflags_t *rflags = (rflags_t *)PyType_GenericNew(&type, NULL, NULL);
    rflags->_if = flags->s._if;
    rflags->ac = flags->s.ac;
    rflags->af = flags->s.af;
    rflags->cf = flags->s.cf;
    rflags->df = flags->s.df;
    rflags->id = flags->s.id;
    rflags->nt = flags->s.nt;
    rflags->of = flags->s.of;
    rflags->pf = flags->s.pf;
    rflags->rf = flags->s.rf;
    rflags->sf = flags->s.sf;
    rflags->tf = flags->s.tf;
    rflags->vif = flags->s.vif;
    rflags->vip = flags->s.vip;
    rflags->vm = flags->s.vm;
    rflags->zf = flags->s.zf;
    return rflags;
}


/* Initialization of `pyxed.Rflags' type should go here. */
static void initialize_rflags_type(PyTypeObject *type)
{
    *(PyObject *)type = type_base;
    type->tp_name = "pyxed.Rflags";
    type->tp_basicsize = sizeof(rflags_t);
    type->tp_flags = Py_TPFLAGS_DEFAULT;
    type->tp_doc = "Represents the CPU flags register contents";
    type->tp_members = members;
}

void register_rflags_object(PyObject *module)
{
    initialize_rflags_type(&type);
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "Rflags", (PyObject *)&type);
    }
}


