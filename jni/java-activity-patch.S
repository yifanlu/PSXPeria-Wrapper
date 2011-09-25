@@
@@ PSXperia Emulator Function Patches
@@ Copyright (C) 2011 Yifan Lu (http://yifan.lu/)
@@ 
@@ This program is free software: you can redistribute it and/or modify
@@ it under the terms of the GNU General Public License as published by
@@ the Free Software Foundation, either version 3 of the License, or
@@ (at your option) any later version.
@@ 
@@ This program is distributed in the hope that it will be useful,
@@ but WITHOUT ANY WARRANTY; without even the implied warranty of
@@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@@ GNU General Public License for more details.
@@ 
@@ You should have received a copy of the GNU General Public License
@@ along with this program.  If not, see <http://www.gnu.org/licenses/>.
@@
@@ You MUST manually change the offsets for "dlsym" and the functions' names after assembling
	.text
@@ Main function is useless, just to inhibit assembler errors
	.align	2
	.global	main
	.type	main, %function
main:
	.fnstart
	mov		r0, #0
	bx		lr
	.fnend
	.size	main, .-main
	.align

@@ int PsCrypt_GetImageToc(void*) patch
PsCrypt_GetImageToc_Patch:
	.type	PsCrypt_GetImageToc_Patch, %function
	.fnstart
	stmfd	sp!, {r0, lr}				@ preserve registers into stack
	.save	{r0, lr}
	ldr		r1, =GetImageToc			@ position of function name string, TODO: Change offset to match that inside the emulator
	mov		r0, #0xFFFFFFFF				@ handle
	bl		dlsym						@ get pointer to function, TODO: Automatically find offset to dlsym in emulator
	mov		r2, r0						@ store function pointer
	ldmfd	sp!, {r0, lr}				@ restore registers
	bx		r2							@ call function, lr should point to original caller, return value in r1
	.fnend
	.size	PsCrypt_GetImageToc_Patch, .-PsCrypt_GetImageToc_Patch
	.align

GetImageToc:
	.ascii "PsCrypt_GetImageToc\000"
	.align
	.ltorg								@ Dump data so each function is in their own section

@@ int PsCrypt_GetImageTocLength() patch	
PsCrypt_GetImageTocLength_Patch:
	.type	PsCrypt_GetImageTocLength_Patch, %function
	.fnstart
	stmfd	sp!, {lr}					@ preserve registers into stack
	.save	{lr}
	ldr		r1, =GetImageTocLength		@ position of function name string, TODO: Change offset to match that inside the emulator
	mov		r0, #0xFFFFFFFF				@ handle
	bl		dlsym						@ get pointer to function, TODO: Automatically find offset to dlsym in emulator
	ldmfd	sp!, {lr}					@ restore registers
	bx		r0							@ call function, lr should point to original caller, return value in r1
	.fnend
	.size	PsCrypt_GetImageTocLength_Patch, .-PsCrypt_GetImageTocLength_Patch
	.align

GetImageTocLength:
	.ascii "PsCrypt_GetImageTocLength\000"
	.align
	.ltorg								@ Dump data so each function is in their own section

.end