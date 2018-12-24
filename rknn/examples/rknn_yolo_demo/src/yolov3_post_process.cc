/****************************************************************************
*
*    Copyright (c) 2017 - 2018 by Rockchip Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Rockchip Corporation. This is proprietary information owned by
*    Rockchip Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Rockchip Corporation.
*
*****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <vector>

#include "yolov3_post_process.h"


float buffer[LISTSIZE *SPAN *(GRID0 *GRID0 +GRID1 *GRID1 +GRID2 *GRID2)];

float result_buffer[63882];

typedef struct _RECT {
	float left;
	float top;
	float right;
	float bottom;
} RECT, *PRECT;

struct Object
{
    RECT rect;
    int label;
    float prob;
};

float my_sigmoid(float x)
{

	return 1/(1 +exp(-x));
}

void process_feats0(float output[1][GRID0][GRID0][SPAN][LISTSIZE], int anchors[SPAN][2], float obj_threshold, 
					float* boxes, int* box_class, float* box_score, int* num_candida)
{
	int num = 0;
	for (int i = 0; i < GRID0; i++){
		for (int j = 0; j < GRID0; j++){
			for (int k = 0; k < SPAN; k++){
				float* data = &output[0][i][j][k][0];
				float confidence = my_sigmoid(data[4]);
				int seq_maxscore = std::max_element(&data[5], &data[5] +80) -&data[5];
				float maxscore = my_sigmoid(data[5 +seq_maxscore]) *confidence;
                if(maxscore < obj_threshold)
                    continue;
				float x = my_sigmoid(data[0]);
				float y = my_sigmoid(data[1]);
				float w = exp(data[2]) *anchors[k][0] /416;
				float h = exp(data[3]) *anchors[k][1] /416;
				x = (x +j)/GRID0 -w/2;
				y = (y +i)/GRID0 -h/2;
				boxes[num*4 +0] = x;
				boxes[num*4 +1] = y;
				boxes[num*4 +2] = w;
				boxes[num*4 +3] = h;
				box_class[num] = seq_maxscore;
				box_score[num] = maxscore;

				num++;
			}
		}
	}
	*num_candida = num;
}

void process_feats1(float output[1][GRID1][GRID1][SPAN][LISTSIZE], int anchors[SPAN][2], float obj_threshold,
					float* boxes, int* box_class, float* box_score, int* num_candida)
{
	int num = 0;
	for (int i = 0; i < GRID1; i++){
		for (int j = 0; j < GRID1; j++){
			for (int k = 0; k < SPAN; k++){
				float* data = &output[0][i][j][k][0];
				float confidence = my_sigmoid(data[4]);
				int seq_maxscore = std::max_element(&data[5], &data[5] +80) -&data[5];
				float maxscore = my_sigmoid(data[5 +seq_maxscore]) *confidence;
				if(maxscore < obj_threshold)
					continue;
				float x = my_sigmoid(data[0]);
				float y = my_sigmoid(data[1]);
				float w = exp(data[2]) *anchors[k][0] /416;
				float h = exp(data[3]) *anchors[k][1] /416;
				x = (x +j)/GRID1 -w/2;
				y = (y +i)/GRID1 -h/2;
				boxes[num*4 +0] = x;
				boxes[num*4 +1] = y;
				boxes[num*4 +2] = w;
				boxes[num*4 +3] = h;
				box_class[num] = seq_maxscore;
				box_score[num] = maxscore;
				num++;
			}
		}
	}
	*num_candida = num;
}

void process_feats2(float output[1][GRID2][GRID2][SPAN][LISTSIZE], int anchors[SPAN][2], float obj_threshold,
					float* boxes, int* box_class, float* box_score, int* num_candida)
{
	int num = 0;
	for (int i = 0; i < GRID2; i++){
		for (int j = 0; j < GRID2; j++){
			for (int k = 0; k < SPAN; k++){
				float* data = &output[0][i][j][k][0];
				float confidence = my_sigmoid(data[4]);
				int seq_maxscore = std::max_element(&data[5], &data[5] +80) -&data[5];
				float maxscore = my_sigmoid(data[5 +seq_maxscore]) *confidence;
				if(maxscore < obj_threshold)
					continue;
				float x = my_sigmoid(data[0]);
				float y = my_sigmoid(data[1]);
				float w = exp(data[2]) *anchors[k][0] /416;
				float h = exp(data[3]) *anchors[k][1] /416;
				x = (x +j)/GRID2 -w/2;
				y = (y +i)/GRID2 -h/2;
				boxes[num*4 +0] = x;
				boxes[num*4 +1] = y;
				boxes[num*4 +2] = w;
				boxes[num*4 +3] = h;
				box_class[num] = seq_maxscore;
				box_score[num] = maxscore;
				num++;
			}
		}
	}
	*num_candida = num;
}

float intersection_area(const Object *a, const Object *b)
{
	float left_ = a->rect.left > b->rect.left ? a->rect.left:b->rect.left;
	float top_ = a->rect.top > b->rect.top ? a->rect.top:b->rect.top;
	float right_ = a->rect.right < b->rect.right ? a->rect.right:b->rect.right;
	float bottom_ = a->rect.bottom < b->rect.bottom ? a->rect.bottom:b->rect.bottom;
	float width = right_ -left_ > 0 ? right_ -left_ : 0;
	float height = bottom_ -top_ > 0 ? bottom_ -top_ : 0;

    return height*width;
}

void qsort_descent_inplace(Object* objects, int left, int right)
{
    int i = left;
    int j = right;
    float p = objects[(left + right) / 2].prob;

    while (i <= j)
    {
        while (objects[i].prob > p)
            i++;

        while (objects[j].prob < p)
            j--;

        if (i <= j)
        {
            // swap
            std::swap(objects[i], objects[j]);

            i++;
            j--;
        }
    }

    if (left < j) qsort_descent_inplace(objects, left, j);
    if (i < right) qsort_descent_inplace(objects, i, right);   
}

void qsort_descent_inplace(Object* objects, int size)
{

    if (size == 0)
        return;

    qsort_descent_inplace(objects, 0, size - 1);
}

int nms_sorted_bboxes(Object * objects, int obj_num, int *picked, float nms_threshold)
{
    const int n = obj_num;

	std::vector<float> areas(n);
    for (int i = 0; i < n; i++)
    {
	areas[i] = (objects[i].rect.bottom - objects[i].rect.top) * (objects[i].rect.right - objects[i].rect.left);
    }

    int picked_size = 0;

    for (int i = 0; i < n; i++)
    {
        const Object a = objects[i];

        int keep = 1;
        for (int j = 0; j < picked_size; j++){
            const Object b = objects[picked[j]];

            // intersection over union
            float inter_area = intersection_area(&a, &b);
            float union_area = areas[i] + areas[picked[j]] - inter_area;
            if (inter_area / union_area > nms_threshold)
                keep = 0;
        }

        if (keep) {
            picked[picked_size++] = i;
        }
    }
    return picked_size;
}


int process(float output0[1][GRID0][GRID0][SPAN][LISTSIZE], float output1[1][GRID1][GRID1][SPAN][LISTSIZE], 
			float* out_pos, float* out_prop, int* out_label)
{
	
	int anchors0[SPAN][2] = {{81,82},  {135,169},  {344,319}};
	int anchors1[SPAN][2] = {{10,14},  {23,27},  {37,58}};

	//three result
	float* boxes = &result_buffer[0];
	int* box_class = (int*)&result_buffer[42588];
	float* box_score = &result_buffer[53235];

	int num_candida0 = 0;
	process_feats0(output0, anchors0, OBJ_THRESH, boxes, box_class, box_score, &num_candida0);
	int num_candida1 = 0;
	process_feats1(output1, anchors1, OBJ_THRESH, &boxes[4 *num_candida0], &box_class[num_candida0], &box_score[num_candida0], &num_candida1);
	int num_candida = num_candida0 +num_candida1;

	if (num_candida == 0){
		return 0;
	}
	if(num_candida > MAX_BOX){
		num_candida = MAX_BOX;
	}
	int out_num = 0;

	Object *allcandidates = (Object*) malloc(num_candida * sizeof(Object));
	//Object(&t_allcandidates)[MAX_BOX] = *((Object(*)[MAX_BOX])allcandidates);
	for (int i = 0; i < num_candida; i++){
		allcandidates[i].rect.left =  boxes[4*i +0];
		allcandidates[i].rect.top =  boxes[4*i +1];
		allcandidates[i].rect.right =  allcandidates[i].rect.left +boxes[4*i +2];
		allcandidates[i].rect.bottom =  allcandidates[i].rect.top +boxes[4*i +3];
		allcandidates[i].prob = box_score[i];
		allcandidates[i].label = box_class[i];
		
	}
	qsort_descent_inplace(allcandidates, num_candida);
	//printf("candidates num: %d\n", num_candida);
	/*for (int ii = 0; ii < num_candida; ii++) {
		printf("%d - %s %f (%.2f %.2f %.2f %.2f)\n", ii, classname[allcandidates[ii].label], allcandidates[ii].prob, allcandidates[ii].rect.left, allcandidates[ii].rect.top, allcandidates[ii].rect.right, allcandidates[ii].rect.bottom);
	}*/
	Object *cla_candidates = (Object*) malloc(num_candida * sizeof(Object));
	//Object(&t_cla_candidates)[MAX_BOX] = *((Object(*)[MAX_BOX])cla_candidates);
	for (int cla = 0; cla < NUM_CLS; cla++) {

		
		int cla_candidate_num = 0;
		for (int i = 0; i < num_candida; i++){
			if(allcandidates[i].label == cla){
				cla_candidates[cla_candidate_num++] = allcandidates[i];
			}
		}
		if(cla_candidate_num == 0)
			continue;
		
		
		int picked[MAX_BOX];
		memset(picked, -1, MAX_BOX);
		int picked_size = nms_sorted_bboxes(cla_candidates, cla_candidate_num, picked, NMS_THRESH);
		for (int j = 0; j < picked_size; j++)
		{
			int z = picked[j];
			if(z < 0)
				printf("post_process_nms_error\n");
			out_pos[out_num * 4 + 0] = cla_candidates[z].rect.left;
			out_pos[out_num * 4 + 1] = cla_candidates[z].rect.top;
			out_pos[out_num * 4 + 2] = cla_candidates[z].rect.right;
			out_pos[out_num * 4 + 3] = cla_candidates[z].rect.bottom;
			out_prop[out_num] = cla_candidates[z].prob;
			out_label[out_num] = cla_candidates[z].label;
			out_num++;
			
		}
		
	}
	free(cla_candidates);
	free(allcandidates);
	/*printf("\n\n");
	for (int i = 0; i < out_num; i++){
		int lab = out_label[i];
		float prob = out_prop[i];
		float left = out_pos[i * 4 + 0];
		float top = out_pos[i * 4 + 1];
		float right = out_pos[i * 4 + 2];
		float down = out_pos[i * 4 + 3];
		printf("%d - %s %f (%.2f %.2f %.2f %.2f)\n",i, classname[lab], prob, left, top, right, down);

	}*/
	return out_num;
}

int yolov3_post_process(float* input0, float* input1, float* out_pos, float* out_prop, int* out_label)
{

	float(&output0)[1][GRID0][GRID0][SPAN][LISTSIZE] = *((float(*)[1][GRID0][GRID0][SPAN][LISTSIZE])buffer);
	float(&output1)[1][GRID1][GRID1][SPAN][LISTSIZE] = *((float(*)[1][GRID1][GRID1][SPAN][LISTSIZE])(buffer +LISTSIZE *SPAN *GRID0 *GRID0));

	for (int i = 0; i < SPAN; i++){
		for (int j = 0; j < LISTSIZE; j++){

			int bias0 = i*LISTSIZE*GRID0*GRID0+ j*GRID0*GRID0;
			for (int k = 0; k < GRID0; k++){
				int bias_k = bias0 + k*GRID0;
				for (int l = 0; l < GRID0; l++){
					output0[0][k][l][i][j] = input0[(bias_k+l)];
				}
			}

			int bias1 = i*LISTSIZE*GRID1*GRID1+ j*GRID1*GRID1;
			for (int k = 0; k < GRID1; k++){
				int bias_k = bias1 + k*GRID1;
				for (int l = 0; l < GRID1; l++){
					output1[0][k][l][i][j] = input1[bias_k+l];
				}
			}

		}
	}

	int object_num = process(output0, output1, out_pos, out_prop, out_label);
	return object_num;
}

