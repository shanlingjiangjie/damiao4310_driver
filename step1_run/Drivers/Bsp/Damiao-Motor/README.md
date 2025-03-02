# 项目介绍
    本项目是一份对达妙电机驱动的使用说明

# 版本内容更新
## v1.0.0: 
    1.对MIT模式进行一些简单说明 

# 环境依赖
**配置文件**
```
#include "buffer_append/buffer_append.h"
#include "can_list/can_list.h"
```
# 使用说明
## MIT模式
下文是针对官方文档进行一些重要信息的截取，详细请参阅
https://gl1po2nscb.feishu.cn/wiki/Y3OEwMr4GivZU9kZqkjctmGinye
### 1.MIT模式说明
MIT 模式可实现 力矩、位置、速度三者混合控制

### 2.MIT模式用法说明
1. 当 kp=0，kd≠0 时，给定 v_des 即可实现匀速转动。匀速转动过程中存在静差，另外 kd 不宜过大， kd 过大时会引起震荡。
2. 当 kp=0，kd=0 时，给定 t_ff 即可实现给定扭矩输出。在该情况下，电机会持续输出一个恒定力矩。但是当电机空转或负载较小时，如果给定 t_ff 较大，电机会持续加速，直到加速到最大速度，这时也仍然达不到目标力矩 t_ff。
3. 当 kp≠0，kd=0 时，会引起震荡。即对位置进行控制时，kd 不能赋0，否则会造成电机震荡，甚至失控。
4. 当 kp≠0，kd≠0 时，有多种情况，这里下面简单介绍两种情况。

- 当期望位置 p_des 为常量，期望速度 v_des 为0时，可实现定点控制，在这个过程中，实际位置趋近于p_des，实际速度 dθm 趋近于0。
- 当p_des是随时间变化的连续可导函数时，同时 v_des 是 p_des 的导数，可实现位置跟踪和速度跟踪，即按照期望速度旋转期望角度。但当电机带有负载时，则需要加入前馈力矩进行补偿。
- PS:经实际测试，当 p_des 为常量， v_des 也为常量时，电机不会发生旋转
## 3.编写代码时的注意事项
1.<mark>当要把电机调至某个模式时，需要先在上位机中进行相应的模式修改

2.CAN配置
```
can1_init(1000,350);
can_list_add_can(can1_selected,4,4);
```
## 4.官方文档总地址
https://gitee.com/kit-miao/damiao
# 代码示例 
```
int main()
{
    dm_handle_t motor_4310;
    can1_init(1000,350);
    can_list_add_ca(can1_selected,4,4);
    dm_motor_init(&motor_43100x00,0x01,DM_MODE_MITDM_J4310,12.5,30,10can1_selected);
    //注意调整模式时要在上位机进对应的修改
    dm_motor_enable(motor_4310);
    dm_save_zero(&motor_4310);
    while(1)
    {
        dm_mit_ctrl(&motor_4310,0,5,0,0,0);
    }  
    return 0;
}
```