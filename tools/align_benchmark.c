/*
 *                             The MIT License
 *
 * Wavefront Alignments Algorithms
 * Copyright (c) 2017 by Santiago Marco-Sola  <santiagomsola@gmail.com>
 *
 * This file is part of Wavefront Alignments Algorithms.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * PROJECT: Wavefront Alignments Algorithms
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: Wavefront Alignments Algorithms benchmarking tool
 */



#define _GNU_SOURCE

#include "utils/commons.h"
#include "system/profiler_timer.h"

#include "edit/edit_table.h"
#include "edit/edit_dp.h"
#include "gap_lineal/nw.h"
#include "gap_affine/affine_wavefront.h"
#include "gap_affine/swg.h"

#include "benchmark/benchmark_edit.h"
#include "benchmark/benchmark_gap_affine.h"
#include "benchmark/benchmark_gap_lineal.h"

#include <pthread.h>
#include <time.h>
#include <sched.h>

#define MAX_LINE 10000000

#define NUM_THREADS 40

long file_size;

/*
 * Algorithms
 */
typedef enum {
  alignment_edit_dp,
  alignment_edit_dp_banded,
  alignment_gap_lineal_nw,
  alignment_gap_affine_swg,
  alignment_gap_affine_swg_banded,
  alignment_gap_affine_wavefront,
} alg_algorithm_type;

/*
 * Generic parameters
 */
typedef struct {
  // Input
  char *algorithm;
  char *input;
  // Penalties
  lineal_penalties_t lineal_penalties;
  affine_penalties_t affine_penalties;
  // Specific parameters
  int bandwidth;
  wavefront_reduction_type reduction_type;
  int min_wavefront_length;
  int max_distance_threshold;
  // Profile
  profiler_timer_t timer_global;
  int progress;
  // Check
  bool check_correct;
  bool check_score;
  bool check_alignments;
  int check_metric;
  int check_bandwidth;
  bool verbose;
} benchmark_args;
benchmark_args parameters = {
  // Input
  .algorithm=NULL,
  .input=NULL,
  // Penalties
  .lineal_penalties = {
      .match = -1,
      .mismatch = 4,
      .insertion = 2,
      .deletion  = 2,
  },
  .affine_penalties = {
      .match = 0,
      .mismatch = 4,
      .gap_opening = 6,
      .gap_extension = 2,
  },
  // Specific parameters
  .bandwidth = 10,
  .reduction_type = wavefronts_reduction_none,
  .min_wavefront_length = 10,
  .max_distance_threshold = 50,
  // Check
  .check_correct = false,
  .check_score = false,
  .check_alignments = false,
  .check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_AFFINE,
  .check_bandwidth = -1,
  .progress = 10000,
  .verbose = false
};

/*
 * Benchmark UTest
 */
void align_pairwise_test() {
  // Patters & Texts
  char* pattern = "TCTTTACTCGCGCGTTGGAGAAATACAATAGT";
  char* text    = "TCTATACTGCGCGTTTGGAGAAATAAAATAGT";
  // Configure align_input
  mm_allocator_t* const mm_allocator = mm_allocator_new(BUFFER_SIZE_8M);
  align_input_t align_input = {
      .sequence_id = 0,
      .pattern = pattern,
      .pattern_length = strlen(pattern),
      .text = text,
      .text_length = strlen(text),
      .mm_allocator = mm_allocator,
      .debug_flags = /* ALIGN_DEBUG_DISPLAY_INFO | */ ALIGN_DEBUG_CHECK_SCORE | ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_AFFINE,
      .verbose = true,
  };
  benchmark_align_input_clear(&align_input);
  /*
   * Edit
   */
  // Edit-DP
  //   benchmark_edit_dp(&align_input);
  //   benchmark_edit_dp_banded(&align_input,5);
  //   benchmark_seqan_global_edit(&align_input);
  /*
   * NW
   */
  //  benchmark_nw_dp(&align_input,&parameters.gap_lineal_penalties);
  /*
   * SWG
   */
  align_input.debug_flags = 0;
  align_input.check_lineal_penalties = &parameters.lineal_penalties;
  align_input.check_affine_penalties = &parameters.affine_penalties;

  parameters.affine_penalties.match = 0;
  parameters.affine_penalties.mismatch = 6;
  parameters.affine_penalties.gap_opening = 5;
  parameters.affine_penalties.gap_extension = 3;

  //  benchmark_gap_affine_swg(&align_input,&parameters.affine_penalties);
  //  benchmark_gap_affine_swg_banded(&align_input,&parameters.affine_penalties,30);
  benchmark_gap_affine_wavefront(
      &align_input,&parameters.affine_penalties,10,50);
  //  benchmark_ksw2_extz2_sse(&align_input,&parameters.affine_penalties,false,1000,100);
  //  benchmark_ksw2_extd2_sse(&align_input,&parameters.affine_penalties,false,-1,-1);
  // Free
  mm_allocator_delete(mm_allocator);
}
/*
 * Benchmark
 */


double what_time_is_it()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}

struct ThreadArgs{
    int* threadID;
    long fileSize;
};

int getNumberOfLines(char *inputString) {
    char *delimiter = "-";
    char *firstDash = strstr(inputString, delimiter);
    char *secondDash = strstr(firstDash + 1, delimiter);
    char *thirdDash = strstr(secondDash + 1, delimiter);
    int value = atoi(thirdDash + 1);
    if (value == 5) {
      value = 10;
    }
    return value * 1000000;
}


int getSecondTokenAsInt(char *inputString) {
    char *delimiter = "-";
    char *firstDash = strstr(inputString, delimiter);

    if (firstDash != NULL) {
        char *secondDash = strstr(firstDash + 1, delimiter);
        if (secondDash != NULL) {
            size_t length = secondDash - (firstDash + 1);
            char *result = malloc(length + 1);
            if (result != NULL) {
                strncpy(result, firstDash + 1, length);
                result[length] = '\0';
                int intValue = atoi(result+1);
                free(result);
                return intValue;
            }
        }
    }

    return 0; // Return 0 if there's no valid second token
}

int moveFilePointerBackwardToBeginingOfLine(FILE *file) {
    while (true) {
        char c = fgetc(file);
        if (c == '>') {
            fseek(file, -1, SEEK_CUR);
            return 0;
        } else if (c == '<') {
            fseek(file, -1, SEEK_CUR);
            return 1;
        }
        fseek(file, -2, SEEK_CUR);
    }
}



void *align(void *args)
{
    int thread_id = *(int *)args;
    //struct ThreadArgs *thread_args = (struct ThreadArgs *)args;
    //ThreadArgs *thread_args = (ThreadArgs *)args;
    //int thread_id = *(thread_args->threadID);
    //printf("thread id is %d and filesize is %ld\n", thread_id, file_size);
    FILE *input_file = NULL;
    char *line1 = NULL, *line2 = NULL;
    int line1_length=0, line2_length=0;
    size_t line1_size=0, line2_size=0;
    align_input_t align_input;
    // Init
    input_file = fopen(parameters.input, "r");
    if (input_file==NULL) {
      fprintf(stderr,"Input file '%s' couldn't be opened\n",parameters.input);
      exit(1);
    }
    benchmark_align_input_clear(&align_input);
    align_input.debug_flags = 0;
    align_input.debug_flags |= parameters.check_metric;
    if (parameters.check_correct) align_input.debug_flags |= ALIGN_DEBUG_CHECK_CORRECT;
    if (parameters.check_score) align_input.debug_flags |= ALIGN_DEBUG_CHECK_SCORE;
    if (parameters.check_alignments) align_input.debug_flags |= ALIGN_DEBUG_CHECK_ALIGNMENT;
    align_input.check_lineal_penalties = &parameters.lineal_penalties;
    align_input.check_affine_penalties = &parameters.affine_penalties;
    align_input.check_bandwidth = parameters.check_bandwidth;
    align_input.verbose = parameters.verbose;
    align_input.mm_allocator = mm_allocator_new(BUFFER_SIZE_8M);
    // Read-align loop
    int reads_processed = 0;
    //int seq_length = getSecondTokenAsInt(parameters.input);
    //int num_lines = getNumberOfLines(parameters.input);
    //long file_size = num_lines * seq_length;
    //int lines_per_thread = (num_lines + NUM_THREADS - 1) / NUM_THREADS;
    //int start_line = thread_id * lines_per_thread;
    //int end_line = (thread_id + 1) * lines_per_thread - 1;
    long bytes_per_thread = (file_size + NUM_THREADS - 1) / NUM_THREADS;
    long start_byte = thread_id * bytes_per_thread;
    long end_byte = (thread_id + 1) * bytes_per_thread - 1;

    // If this is the last thread, include the remaining lines
    if (thread_id == NUM_THREADS - 1) {
        //end_line = num_lines - 1;
        end_byte = file_size;
        //printf("FileSize is %ld\n", thread_args->fileSize);
    }
    //int current_line = start_line;
    long current_byte = start_byte;
    //char line[550];
    //fseek(input_file, start_line*102, SEEK_SET);
    fseek(input_file, end_byte, SEEK_SET);
    if (moveFilePointerBackwardToBeginingOfLine(input_file) == 0) {
        fseek(input_file, -1, SEEK_CUR);
        moveFilePointerBackwardToBeginingOfLine(input_file);
        //end_byte = ftell(input_file);
        //printf("end_byte is %ld\n", end_byte);
    }
    end_byte = ftell(input_file);
    fseek(input_file, start_byte, SEEK_SET);
    //line1_length = getline(&line1, &line1_size, input_file);
    //printf("thread id %d\n", thread_id);
    //printf("thread id %d line1 is %s\n", thread_id, line1);
    if (moveFilePointerBackwardToBeginingOfLine(input_file) == 1) {
        //printf("line1 length is %ld\n", line1_size);
        line1_length = getline(&line1, &line1_size, input_file);
        //fseek(input_file, start_byte, SEEK_SET);
    }
    start_byte = ftell(input_file);
    //printf("my id is %d\n", thread_id);

    //timer_reset(&align_input.timer);
    // Read the portion of the file
    //while (current_line <= end_line) {
    while (current_byte <= end_byte) {
       // Read queries
       line1_length = getline(&line1, &line1_size, input_file);
       line2_length = getline(&line2, &line2_size, input_file);
       if (line1_length == -1 || line2_length == -1) break;
       // Configure input
       //align_input.sequence_id = reads_processed;
       align_input.pattern = line1+1;
       align_input.pattern_length = line1_length-2;
       align_input.pattern[align_input.pattern_length] = '\0';
       align_input.text = line2+1;
       align_input.text_length = line2_length-2;
       align_input.text[align_input.text_length] = '\0';
       benchmark_gap_affine_wavefront(
           &align_input,&parameters.affine_penalties,
           parameters.min_wavefront_length,
           parameters.max_distance_threshold);
       reads_processed += 1;
       //current_line+=2;
       current_byte = ftell(input_file);
    } //while

    if (parameters.check_correct || parameters.check_score || parameters.check_alignments) {
      //const bool print_wf_stats = (alg_algorithm == alignment_gap_affine_wavefront);
      benchmark_print_stats(stderr,&align_input,true);
    }

    //fprintf(stderr,"  => Time.Alignment    ");
    //timer_print(stderr,&align_input.timer,&parameters.timer_global);
    fclose(input_file);
    mm_allocator_delete(align_input.mm_allocator);
    free(line1);
    free(line2);
    //printf("Thread %d is running \n", thread_id);
    return NULL;
}



void align_benchmark(const alg_algorithm_type alg_algorithm) {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    //double time = what_time_is_it();

    FILE *input_file = NULL;
    input_file = fopen(parameters.input, "r");
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    //rewind(input_file);
    //struct ThreadArgs args;
    //args.fileSize = file_size;


    timer_restart(&(parameters.timer_global));
    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        //args.threadID = &thread_ids[i];
        //pthread_create(&threads[i], NULL, align, &args);
        pthread_create(&threads[i], NULL, align, &thread_ids[i]);
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    timer_stop(&(parameters.timer_global));
    //fprintf(stderr,"  => Time.Benchmark  %f \n", what_time_is_it() - time);
    timer_print(stderr,&parameters.timer_global,NULL);
    // Print the word count
}


//void align_benchmark(const alg_algorithm_type alg_algorithm) {
//  // Parameters
//  FILE *input_file = NULL;
//  char *line1 = NULL, *line2 = NULL;
//  int line1_length=0, line2_length=0;
//  size_t line1_allocated=0, line2_allocated=0;
//  align_input_t align_input;
//  // Init
//  timer_restart(&(parameters.timer_global));
//  input_file = fopen(parameters.input, "r");
//  if (input_file==NULL) {
//    fprintf(stderr,"Input file '%s' couldn't be opened\n",parameters.input);
//    exit(1);
//  }
//  benchmark_align_input_clear(&align_input);
//  align_input.debug_flags = 0;
//  align_input.debug_flags |= parameters.check_metric;
//  if (parameters.check_correct) align_input.debug_flags |= ALIGN_DEBUG_CHECK_CORRECT;
//  if (parameters.check_score) align_input.debug_flags |= ALIGN_DEBUG_CHECK_SCORE;
//  if (parameters.check_alignments) align_input.debug_flags |= ALIGN_DEBUG_CHECK_ALIGNMENT;
//  align_input.check_lineal_penalties = &parameters.lineal_penalties;
//  align_input.check_affine_penalties = &parameters.affine_penalties;
//  align_input.check_bandwidth = parameters.check_bandwidth;
//  align_input.verbose = parameters.verbose;
//  align_input.mm_allocator = mm_allocator_new(BUFFER_SIZE_8M);
//  timer_reset(&align_input.timer);
//  // Read-align loop
//  int reads_processed = 0, progress = 0;
//
//   while (true) {
//    // Read queries
//    line1_length = getline(&line1, &line1_allocated, input_file);
//    line2_length = getline(&line2, &line2_allocated, input_file);
//    if (line1_length == -1 || line2_length == -1) break;
//    // Configure input
//    align_input.sequence_id = reads_processed;
//    align_input.pattern = line1+1;
//    align_input.pattern_length = line1_length-2;
//    align_input.pattern[align_input.pattern_length] = '\0';
//    align_input.text = line2+1;
//    align_input.text_length = line2_length-2;
//    align_input.text[align_input.text_length] = '\0';
//    // Align queries using DP
//    switch (alg_algorithm) {
//      case alignment_edit_dp:
//        benchmark_edit_dp(&align_input);
//        break;
//      case alignment_edit_dp_banded:
//        benchmark_edit_dp_banded(&align_input,parameters.bandwidth);
//        break;
//      case alignment_gap_lineal_nw:
//        benchmark_gap_lineal_nw(&align_input,&parameters.lineal_penalties);
//        break;
//      case alignment_gap_affine_swg:
//        benchmark_gap_affine_swg(&align_input,&parameters.affine_penalties);
//        break;
//      case alignment_gap_affine_swg_banded:
//        benchmark_gap_affine_swg_banded(&align_input,
//            &parameters.affine_penalties,parameters.bandwidth);
//        break;
//      case alignment_gap_affine_wavefront:
//        benchmark_gap_affine_wavefront(
//            &align_input,&parameters.affine_penalties,
//            parameters.min_wavefront_length,
//            parameters.max_distance_threshold);
//        break;
//        printf("line1_length %d\n", line1_length);
//      default:
//        fprintf(stderr,"Algorithm unknown or not implemented\n");
//        exit(1);
//        break;
//    }
//    // Update progress
//    ++reads_processed;
//    // DEBUG mm_allocator_print(stderr,align_input.mm_allocator,true);
//    if (++progress == parameters.progress) {
//      progress = 0;
//      // Compute speed
//      const uint64_t time_elapsed_global = timer_elapsed_ns(&(parameters.timer_global));
//      const float rate_global = (float)reads_processed/(float)TIMER_CONVERT_NS_TO_S(time_elapsed_global);
//      const uint64_t time_elapsed_alg = timer_elapsed_ns(&(align_input.timer));
//      const float rate_alg = (float)reads_processed/(float)TIMER_CONVERT_NS_TO_S(time_elapsed_alg);
//      //fprintf(stderr,"...processed %d reads (benchmark=%2.3f reads/s;alignment=%2.3f reads/s)\n",reads_processed,rate_global,rate_alg);
//    }
//  } //while
//
//
//  timer_stop(&(parameters.timer_global));
//  // Print benchmark results
//  //fprintf(stderr,"[Benchmark]\n");
//  //fprintf(stderr,"=> Total.reads            %d\n",reads_processed);
//  fprintf(stderr,"=> Time.Benchmark      ");
//  timer_print(stderr,&parameters.timer_global,NULL);
//  //fprintf(stderr,"  => Time.Alignment    ");
//  //timer_print(stderr,&align_input.timer,&parameters.timer_global);
//  // Print Stats
//  if (parameters.check_correct || parameters.check_score || parameters.check_alignments) {
//    const bool print_wf_stats = (alg_algorithm == alignment_gap_affine_wavefront);
//    benchmark_print_stats(stderr,&align_input,print_wf_stats);
//  }
//  // Free
//  fclose(input_file);
//  mm_allocator_delete(align_input.mm_allocator);
//  free(line1);
//  free(line2);
//}



/*
 * Generic Menu
 */
void usage() {
  fprintf(stderr,
      "USE: ./align_benchmark -a <algorithm> -i <input>                     \n"
      "      Options::                                                      \n"
      "        [Input]                                                      \n"
      "          --algorithm|a <algorithm>                                  \n"
      "            [edit]                                                   \n"
      "              edit-dp                                                \n"
      "              edit-dp-banded                                         \n"
      "            [gap-lineal]                                             \n"
      "              gap-lineal-nw                                          \n"
      "            [gap-affine]                                             \n"
      "              gap-affine-swg                                         \n"
      "              gap-affine-swg-banded                                  \n"
      "              gap-affine-wfa                                         \n"
      "              gap-affine-wfa-adaptive                                \n"
      "          --input|i <File>                                           \n"
      "        [Penalties]                                                  \n"
      "          --lineal-penalties|p M,X,I,D                               \n"
      "          --affine-penalties|g M,X,O,E                               \n"
      "        [Specifics]                                                  \n"
      "          --bandwidth <INT>                                          \n"
      "          --minimum-wavefront-length <INT>                           \n"
      "          --maximum-difference-distance <INT>                        \n"
      "        [Misc]                                                       \n"
      "          --progress|P <integer>                                     \n"
      "          --check|c 'correct'|'score'|'alignment'                    \n"
      "          --check-distance 'edit'|'gap-lineal'|'gap-affine'          \n"
      "          --check-bandwidth <INT>                                    \n"
      "          --help|h                                                   \n");
}
void parse_arguments(int argc,char** argv) {
  struct option long_options[] = {
    /* Input */
    { "algorithm", required_argument, 0, 'a' },
    { "input", required_argument, 0, 'i' },
    /* Penalties */
    { "lineal-penalties", required_argument, 0, 'p' },
    { "affine-penalties", required_argument, 0, 'g' },
    /* Specifics */
    { "bandwidth", required_argument, 0, 1000 },
    { "minimum-wavefront-length", required_argument, 0, 1002 },
    { "maximum-difference-distance", required_argument, 0, 1003 },
    /* Misc */
    { "progress", required_argument, 0, 'P' },
    { "check", optional_argument, 0, 'c' },
    { "check-distance", required_argument, 0, 2000 },
    { "check-bandwidth", required_argument, 0, 2001 },
    { "verbose", no_argument, 0, 'v' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 } };
  int c,option_index;
  if (argc <= 1) {
    usage();
    exit(0);
  }
  while (1) {
    c=getopt_long(argc,argv,"a:i:p:g:P:c:vh",long_options,&option_index);
    if (c==-1) break;
    switch (c) {
    /*
     * Input
     */
    case 'a':
      parameters.algorithm = optarg;
      break;
    case 'i':
      parameters.input = optarg;
      break;
    /*
     * Penalties
     */
    case 'p': { // --lineal-penalties
      char* sentinel = strtok(optarg,",");
      parameters.lineal_penalties.match = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.lineal_penalties.mismatch = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.lineal_penalties.insertion = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.lineal_penalties.deletion = atoi(sentinel);
      break;
    }
    case 'g': { // --affine-penalties
      char* sentinel = strtok(optarg,",");
      parameters.affine_penalties.match = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.affine_penalties.mismatch = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.affine_penalties.gap_opening = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.affine_penalties.gap_extension = atoi(sentinel);
      break;
    }
    /*
     * Specific parameters
     */
    case 1000: // --bandwidth
      parameters.bandwidth = atoi(optarg);
      break;
    case 1002: // --minimum-wavefront-length
      parameters.min_wavefront_length = atoi(optarg);
      break;
    case 1003: // --maximum-difference-distance
      parameters.max_distance_threshold = atoi(optarg);
      break;
    /*
     * Misc
     */
    case 'P':
      parameters.progress = atoi(optarg);
      break;
    case 'c':
      if (optarg ==  NULL) { // default = score
        parameters.check_correct = true;
        parameters.check_score = true;
        parameters.check_alignments = false;
      } else if (strcasecmp(optarg,"correct")==0) {
        parameters.check_correct = true;
        parameters.check_score = false;
        parameters.check_alignments = false;
      } else if (strcasecmp(optarg,"score")==0) {
        parameters.check_correct = true;
        parameters.check_score = true;
        parameters.check_alignments = false;
      } else if (strcasecmp(optarg,"alignment")==0) {
        parameters.check_correct = true;
        parameters.check_score = true;
        parameters.check_alignments = true;
      } else {
        fprintf(stderr,"Option '--check' must be in {'correct','score','alignment'}\n");
        exit(1);
      }
      break;
    case 2000: // --check-distance
      if (strcasecmp(optarg,"edit")==0) { // default = edit
        parameters.check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_EDIT;
      } else if (strcasecmp(optarg,"gap-lineal")==0) {
        parameters.check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_LINEAL;
      } else if (strcasecmp(optarg,"gap-affine")==0) {
        parameters.check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_AFFINE;
      } else {
        fprintf(stderr,"Option '--check-distance' must be in {'edit','gap-lineal','gap-affine'}\n");
        exit(1);
      }
      break;
    case 2001: // --check-bandwidth
      parameters.check_bandwidth = atoi(optarg);
      break;
    case 'v':
      parameters.verbose = true;
      break;
    case 'h':
      usage();
      exit(1);
    // Other
    case '?': default:
      fprintf(stderr,"Option not recognized \n");
      exit(1);
    }
  }
  // Checks
  if (parameters.algorithm==NULL) {
    fprintf(stderr,"Option --algorithm is required \n");
    exit(1);
  }
  if (strcmp(parameters.algorithm,"test")!=0 && parameters.input==NULL) {
    fprintf(stderr,"Option --input is required \n");
    exit(1);
  }
}
int main(int argc,char* argv[]) {
  // Parsing command-line options
  parse_arguments(argc,argv);
  // Select option
  if (strcmp(parameters.algorithm,"test")==0) {
    align_pairwise_test();
  /* Edit */
  } else if (strcmp(parameters.algorithm,"edit-dp")==0) {
    align_benchmark(alignment_edit_dp);
  } else if (strcmp(parameters.algorithm,"edit-dp-banded")==0) {
    align_benchmark(alignment_edit_dp_banded);
  /* NW */
  } else if (strcmp(parameters.algorithm,"gap-lineal-nw")==0) {
    align_benchmark(alignment_gap_lineal_nw);
  /* SWG */
  } else if (strcmp(parameters.algorithm,"gap-affine-swg")==0) {
    align_benchmark(alignment_gap_affine_swg);
  } else if (strcmp(parameters.algorithm,"gap-affine-swg-banded")==0) {
    align_benchmark(alignment_gap_affine_swg_banded);
  } else if (strcmp(parameters.algorithm,"gap-affine-wfa")==0) {
    parameters.reduction_type = wavefronts_reduction_none;
    parameters.min_wavefront_length = -1;
    parameters.max_distance_threshold = -1;
    align_benchmark(alignment_gap_affine_wavefront);
  } else if (strcmp(parameters.algorithm,"gap-affine-wfa-adaptive")==0) {
    parameters.reduction_type = wavefronts_reduction_dynamic;
    align_benchmark(alignment_gap_affine_wavefront);
  } else {
    fprintf(stderr,"Algorithm '%s' not recognized\n",parameters.algorithm);
    exit(1);
  }
}
