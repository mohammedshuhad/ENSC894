; ModuleID = '/local-scratch/localhome/msa325/ensc894/lab3/Optimized_4096/mm.prj/solution3/.autopilot/db/a.g.ld.5.gdce.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-i64:64-i128:128-i256:256-i512:512-i1024:1024-i2048:2048-i4096:4096-n8:16:32:64-S128-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "fpga64-xilinx-none"

; Function Attrs: noinline
define void @apatb_mm_ir(float* %C, float* %A, float* %B, float %alpha, float %beta) local_unnamed_addr #0 {
entry:
  %malloccall = tail call i8* @malloc(i64 65536)
  %C_copy = bitcast i8* %malloccall to [16384 x float]*
  %malloccall1 = tail call i8* @malloc(i64 65536)
  %A_copy = bitcast i8* %malloccall1 to [16384 x float]*
  %malloccall2 = tail call i8* @malloc(i64 65536)
  %B_copy = bitcast i8* %malloccall2 to [16384 x float]*
  %0 = bitcast float* %C to [16384 x float]*
  %1 = bitcast float* %A to [16384 x float]*
  %2 = bitcast float* %B to [16384 x float]*
  call fastcc void @copy_in([16384 x float]* %0, [16384 x float]* %C_copy, [16384 x float]* %1, [16384 x float]* %A_copy, [16384 x float]* %2, [16384 x float]* %B_copy)
  %3 = getelementptr inbounds [16384 x float], [16384 x float]* %C_copy, i32 0, i32 0
  %4 = getelementptr inbounds [16384 x float], [16384 x float]* %A_copy, i32 0, i32 0
  %5 = getelementptr inbounds [16384 x float], [16384 x float]* %B_copy, i32 0, i32 0
  call void @apatb_mm_hw(float* %3, float* %4, float* %5, float %alpha, float %beta)
  call fastcc void @copy_out([16384 x float]* %0, [16384 x float]* %C_copy, [16384 x float]* %1, [16384 x float]* %A_copy, [16384 x float]* %2, [16384 x float]* %B_copy)
  tail call void @free(i8* %malloccall)
  tail call void @free(i8* %malloccall1)
  tail call void @free(i8* %malloccall2)
  ret void
}

declare noalias i8* @malloc(i64) local_unnamed_addr

; Function Attrs: argmemonly noinline
define internal fastcc void @copy_in([16384 x float]* readonly, [16384 x float]* noalias, [16384 x float]* readonly, [16384 x float]* noalias, [16384 x float]* readonly, [16384 x float]* noalias) unnamed_addr #1 {
entry:
  call fastcc void @onebyonecpy_hls.p0a16384f32([16384 x float]* %1, [16384 x float]* %0)
  call fastcc void @onebyonecpy_hls.p0a16384f32([16384 x float]* %3, [16384 x float]* %2)
  call fastcc void @onebyonecpy_hls.p0a16384f32([16384 x float]* %5, [16384 x float]* %4)
  ret void
}

; Function Attrs: argmemonly noinline
define internal fastcc void @onebyonecpy_hls.p0a16384f32([16384 x float]* noalias, [16384 x float]* noalias readonly) unnamed_addr #2 {
entry:
  %2 = icmp eq [16384 x float]* %0, null
  %3 = icmp eq [16384 x float]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %copy
  %for.loop.idx3 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr.gep1 = getelementptr [16384 x float], [16384 x float]* %0, i64 0, i64 %for.loop.idx3
  %5 = bitcast float* %dst.addr.gep1 to i8*
  %src.addr.gep2 = getelementptr [16384 x float], [16384 x float]* %1, i64 0, i64 %for.loop.idx3
  %6 = bitcast float* %src.addr.gep2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %5, i8* align 1 %6, i64 4, i1 false)
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx3, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 16384
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop, %entry
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #3

; Function Attrs: argmemonly noinline
define internal fastcc void @copy_out([16384 x float]*, [16384 x float]* noalias readonly, [16384 x float]*, [16384 x float]* noalias readonly, [16384 x float]*, [16384 x float]* noalias readonly) unnamed_addr #4 {
entry:
  call fastcc void @onebyonecpy_hls.p0a16384f32([16384 x float]* %0, [16384 x float]* %1)
  call fastcc void @onebyonecpy_hls.p0a16384f32([16384 x float]* %2, [16384 x float]* %3)
  call fastcc void @onebyonecpy_hls.p0a16384f32([16384 x float]* %4, [16384 x float]* %5)
  ret void
}

declare void @free(i8*) local_unnamed_addr

declare void @apatb_mm_hw(float*, float*, float*, float, float)

define void @mm_hw_stub_wrapper(float*, float*, float*, float, float) #5 {
entry:
  %5 = bitcast float* %0 to [16384 x float]*
  %6 = bitcast float* %1 to [16384 x float]*
  %7 = bitcast float* %2 to [16384 x float]*
  call void @copy_out([16384 x float]* null, [16384 x float]* %5, [16384 x float]* null, [16384 x float]* %6, [16384 x float]* null, [16384 x float]* %7)
  %8 = bitcast [16384 x float]* %5 to float*
  %9 = bitcast [16384 x float]* %6 to float*
  %10 = bitcast [16384 x float]* %7 to float*
  call void @mm_hw_stub(float* %8, float* %9, float* %10, float %3, float %4)
  call void @copy_in([16384 x float]* null, [16384 x float]* %5, [16384 x float]* null, [16384 x float]* %6, [16384 x float]* null, [16384 x float]* %7)
  ret void
}

declare void @mm_hw_stub(float*, float*, float*, float, float)

attributes #0 = { noinline "fpga.wrapper.func"="wrapper" }
attributes #1 = { argmemonly noinline "fpga.wrapper.func"="copyin" }
attributes #2 = { argmemonly noinline "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #3 = { argmemonly nounwind }
attributes #4 = { argmemonly noinline "fpga.wrapper.func"="copyout" }
attributes #5 = { "fpga.wrapper.func"="stub" }

!llvm.dbg.cu = !{}
!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3}
!blackbox_cfg = !{!4}

!0 = !{!"clang version 7.0.0 "}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{}
