

#include "gurobi.h"
#include <stdlib.h>
#include "board.h"
ERROR createVarArr(VAR **varArrP, int varCount, int N, board* pBoard);

void cleanUp(int *ind, double *val, double *lowerBound, double *upperBound, double *obj, char *vtype, VAR *varArr,
             GRBenv *env, GRBmodel *model);

void fillArrWithDouble(double *arr, int arrSize, double val);

void setVtype(char *vtype, int len, char type);

ERROR addCellConstraints(VAR **varArrP, GRBmodel *model, int *ind, double *val, int varCount);

void fillVals(double *val, int valSize);

ERROR addRowConstraints(GRBmodel *model, VAR **varArr, int *ind, double *val, int varCount, board *pBoard);

ERROR addColConstraints(GRBmodel *model, VAR **varArr, int *ind, double *val, int varCount, board *pBoard);

ERROR addBlockConstraints(GRBmodel *model, VAR **varArr, int *ind, double *val, int varCount, board *pBoard);

void createRandomObjFunction(double *obj);

ERROR setUpGurobi(board *pBoard, int ilp) {
    GRBenv *env;
    GRBmodel *model;
    ERROR error;
    int N, varCount, grbError;
    int * ind;
    double * val, *lb, *ub, *obj;
    char *vtype;
    VAR* varArr;
    env = NULL;
    model = NULL;
    ind = NULL;
    val = lb = ub = obj = NULL;
    vtype = NULL;
    varArr = NULL;
    N = pBoard->squareSideSize;
    varCount = countVars(pBoard, N);
    /* Create environment - log file is mip1.log */
    grbError = DEBUG ? GRBloadenv(&env, "mip1.log") : GRBloadenv(&env, NULL);
    if (grbError) {
        printf("ERROR %d GRBloadenv(): %s\n", grbError, GRBgeterrormsg(env));
        return GUROBI_ERROR;
    }

    grbError = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (grbError) {
        printf("ERROR %d GRBsetintattr(): %s\n", grbError, GRBgeterrormsg(env));
        return GUROBI_ERROR;
    }



    /*Allocate and check malloc success */
    ind = (int *) malloc(varCount * sizeof(int)); /*TODO: maybe ind and val only need N size? max constraint size*/
    if (ind == NULL){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return MALLOC_ERROR;
    }
    val = (double *) malloc(varCount * sizeof(double));
    if (val == NULL) {
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return MALLOC_ERROR;
    }
    /*Constraints are always with*/
    fillVals(val, N);
    obj = (double *) malloc(varCount * sizeof(double));
    if (obj == NULL){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return MALLOC_ERROR;
    }
    vtype = (char *) malloc(varCount * sizeof(char));
    if (vtype == NULL){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return MALLOC_ERROR;
    }
    lb = (double *) malloc(varCount * sizeof(double));
    if (lb == NULL){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return MALLOC_ERROR;
    }
    ub = (double *) malloc(varCount * sizeof(double));
    if (ub == NULL){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return MALLOC_ERROR;
    }
    varArr = (VAR *) malloc(varCount * sizeof(VAR));
    if (varArr == NULL){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return MALLOC_ERROR;
    }
    error = createVarArr(&varArr, varCount, N, pBoard);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return error;
    }
    /* Create an empty model named "sudoku" */
    if (ilp) {
        setVtype(vtype, varCount, GRB_BINARY);
        createRandomObjFunction(obj); /*TODO: implement. Perhaps not random, but garbage obj?*/
        grbError = GRBnewmodel(env, &model, "sudoku", varCount, obj, NULL, NULL, vtype, NULL);
    }
    else{
        setVtype(vtype, varCount, GRB_CONTINUOUS);
        fillArrWithDouble(lb, varCount, 0.0);
        fillArrWithDouble(ub, varCount, 1.0);
        createRandomObjFunction(obj);
        grbError = GRBnewmodel(env, &model, "sudoku", varCount, obj, lb, ub, vtype, NULL);
    }
    if (grbError) {
        printf("ERROR %d GRBnewmodel(): %s\n", grbError, GRBgeterrormsg(env));
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    grbError = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (grbError) {
        printf("ERROR %d GRBsetintattr(): %s\n", grbError, GRBgeterrormsg(env));
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    grbError = GRBupdatemodel(model);
    if (grbError) {
        printf("ERROR %d GRBupdatemodel(): %s\n", grbError, GRBgeterrormsg(env));
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    error = addCellConstraints(&varArr, model, ind, val, varCount);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    error = addRowConstraints(model, &varArr, ind, val, varCount, pBoard);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    error = addColConstraints(model, &varArr, ind, val, varCount, pBoard);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    error = addBlockConstraints(model, &varArr, ind, val, varCount, pBoard);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    grbError = GRBupdatemodel(model);
    if (grbError) {
        printf("ERROR %d GRBupdatemodel(): %s\n", grbError, GRBgeterrormsg(env));
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    /*TODO: finished adding constraints, optimize and return*/
    cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
    return error;

}

void createRandomObjFunction(double *obj) {
    obj[0] = 0; /*TODO: changeme*/

}

/**
 * Adds block constraints to the model.
 * @param model
 * @param varArr
 * @param ind
 * @param val
 * @param varCount
 * @param pBoard
 * @return
 */
ERROR addBlockConstraints(GRBmodel *model, VAR **varArr, int *ind, double *val, int varCount, board *pBoard) {
    int rowLen, colLen, blockRow, blockCol, index, i,v, N, numOfCols, numOfRows;
    rowLen = pBoard->rows;
    colLen = pBoard->columns;
    N = pBoard->squareSideSize;
    numOfRows = colLen;
    numOfCols = rowLen;
    for (blockRow = 0; blockRow < numOfRows; blockRow++) {
        for (blockCol = 0; blockCol < numOfCols; blockCol++) {
            for (v = 1; v < N + 1; v++) {
                index = 0;
                for (i = 0; i < varCount; i++) {
                    if ((varArr[i]->row / rowLen) == blockRow &&
                        (varArr[i]->col / colLen) == blockCol &&
                         varArr[i]->val == v){
                        ind[index] = i;
                        index++;
                    }
                }
                if (index > 0){
                    GRBaddconstr(model, index, ind, val, GRB_EQUAL, 1.0, NULL);
                }
            }
        }
    }
    return NO_ERROR;
}
/**
 * Adds row constraints to the model.
 * @param model
 * @param varArr
 * @param ind
 * @param val
 * @param varCount
 * @param pBoard
 * @return appropriate error code
 */
ERROR addRowConstraints(GRBmodel *model, VAR **varArr, int *ind, double *val, int varCount, board *pBoard) {
    int N, row, index, i,v;
    N = pBoard->squareSideSize;
    for (row = 0; row < N; row++) {
        for (v = 1; v < N + 1; v++) {
            index = 0;
            for (i = 0; i < varCount; i++) {
                if (varArr[i]->row == row && varArr[i]->val == v){
                    ind[index] = i;
                    index++;
                }
            }
            if (index > 0){
                GRBaddconstr(model, index, ind, val, GRB_EQUAL, 1.0, NULL);
            }
        }
    }
    return NO_ERROR;
}
/**
 * Adds col constraints to the model.
 * @param model
 * @param varArr
 * @param ind
 * @param val
 * @param varCount
 * @param pBoard
 * @return appropriate error code
 */
ERROR addColConstraints(GRBmodel *model, VAR **varArr, int *ind, double *val, int varCount, board *pBoard){
    int N, col, index, i,v;
    N = pBoard->squareSideSize;
    for (col = 0; col < N; col++) {
        for (v = 1; v < N + 1; v++) {
            index = 0;
            for (i = 0; i < varCount; i++) {
                if (varArr[i]->col == col && varArr[i]->val == v){
                    ind[index] = i;
                    index++;
                }
            }
            if (index > 0){
                GRBaddconstr(model, index, ind, val, GRB_EQUAL, 1.0, NULL);
            }
        }
    }
    return NO_ERROR;
}

/**
 * Fills val array (for constraints) with 1's
 * @param val
 * @param valSize
 */
void fillVals(double *val, int valSize) {
    int i;
    for (i = 0; i < valSize; i++) {
        val[i] = 1.0;
    }
}

/**
 * Adds cell constraints to the model, i.e. that every cell has exactly one value.
 * Assumes varArr is sorted by row, then col, then value.
 * @param varArrP
 * @param model
 * @param ind
 * @param val
 * @param N
 * @param varCount
 * @return
 */
ERROR addCellConstraints(VAR **varArrP, GRBmodel *model, int *ind, double *val, int varCount) {
    /*TODO: double check this logic*/
    int i, index, row, col, numOfVarsInConstraint, grbError;
    for (i = 0; i < varCount;) {
        numOfVarsInConstraint = 0;
        index = 0;
        row = varArrP[i]->row;
        col = varArrP[i]->col;
        for (;varArrP[i]->row == row && varArrP[i]->col == col; i++) { /*Since variables are in order of row and col*/
            ind[index] = i;
            index++;
            numOfVarsInConstraint++;
        }
        /*sum_{i \in ind} x_i = 1*/
        grbError = GRBaddconstr(model, numOfVarsInConstraint, ind, val, GRB_EQUAL, 1.0, NULL);
        if (grbError) return GUROBI_ERROR;
    }
    return NO_ERROR;
}

void setVtype(char *vtype, int len, char type) {
    int i;
    for (i = 0; i < len; i++) {
        vtype[i] = type;
    }
}

void fillArrWithDouble(double *arr, int arrSize, double val) {
    int i;
    for (i = 0; i < arrSize; i++) {
        arr[i] = val;
    }
}

void
cleanUp(int *ind, double *val, double *lowerBound, double *upperBound, double *obj, char *vtype, VAR *varArr,
        GRBenv *env, GRBmodel *model) {
    free(ind);
    free(val);
    free(obj);
    free(vtype);
    free(lowerBound);
    free(upperBound);
    free(varArr);
    if (env != NULL)
        GRBfreeenv(env);
    if (model != NULL)
        GRBfreemodel(model);
}

/**
 * Creates the array of VAR-type variables for sudoku
 * @param varArrP pre-allocated array
 * @param varCount size of this array
 */
ERROR createVarArr(VAR **varArrP, int varCount, int N, board *pBoard) {
    int i, j, v, index;
    index = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (emptyCell(pBoard, i, j)) {
                for (v = 1; v < N + 1; v++) {/*Values are 1-indexed*/
                    if (validAsignment(pBoard, v, i, j)) {
                        if (index > varCount) {
                            if (DEBUG) printf("VarCount exceeded in creating varArrP\n");
                            return GUROBI_ERROR; /*Shouldn't happen */
                        }
                        varArrP[index]->row = i;
                        varArrP[index]->col = j;
                        varArrP[index]->val = v;
                        index++;
                    }
                }
            }
        }
    }
    return NO_ERROR;
}

ERROR solveGurobi(board* pBoard, int ilp){
    if (!pBoard || !ilp) return NO_ERROR;
    return NO_ERROR; /*TODO: Changme*/
}
int countVars(board *pBoard, int N) {
    int i, j,k, varCount;
    varCount = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (emptyCell(pBoard, i, j)) {
                for (k = 1; k < N+1; k++) { /*Values are 1-indexed*/
                    if (validAsignment(pBoard, k, i, j)) {
                        varCount++;
                    }
                }
            }
        }
    }
    return varCount;
}

