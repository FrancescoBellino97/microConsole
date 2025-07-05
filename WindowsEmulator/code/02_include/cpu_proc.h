/**
 ******************************************************************************
 * @file    cpu_proc.h
 * @author  Bellino Francesco
 * @brief   CPU process header file.
 *
 *
 ******************************************************************************
 */

#ifndef CPU_PROC_H_
#define CPU_PROC_H_

typedef void (*IN_PROC)(cpu_context *);
IN_PROC inst_get_processor(in_type type);

#endif
