#ifndef SIMM_EMITTER_TASK_H_ 
#define SIMM_EMITTER_TASK_H_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

	void * emitter_task(void *arg);
	int16_t velXReadFromEmitter();
	int16_t gyroThetaZReadFromEmitter();

#ifdef __cplusplus
}
#endif

#endif
