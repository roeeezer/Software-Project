
#include "gurobi.h"
#include <stdlib.h>
#include "board.h"
ERROR createVarArr(VAR *varArr, int varCount, int N, board* pBoard);

void cleanUp(int *ind, double *val, double *lowerBound, double *upperBound, double *obj, char *vtype, VAR *varArr,
             GRBenv *env, GRBmodel *model);

void fillArrWithDouble(double *arr, int arrSize, double val);

void setVtype(char *vtype, int len, char type);

ERROR addCellConstraints(VAR *varArr, GRBmodel *model, int *ind, double *val, int varCount);

void fillVals(double *val, int valSize);

ERROR addRowConstraints(GRBmodel *model, VAR *varArr, int *ind, double *val, int varCount, board *pBoard);

ERROR addColConstraints(GRBmodel *model, VAR *varArr, int *ind, double *val, int varCount, board *pBoard);

ERROR addBlockConstraints(GRBmodel *model, VAR *varArr, int *ind, double *val, int varCount, board *pBoard);

void createRandomObjFunction(double *obj, int varCount);

ERROR fillBoardFromSol(board *pBoard, VAR *varArr, int varCount, double *sol);

/**
 * Gets a sudoku board and *writes the solution to it*
 * @param pBoard
 * @param ilp boolean: is the solution ILP or LP?
 * @return
 */
ERROR setUpGurobi(board *pBoard, int ilp) {
    GRBenv *env;
    GRBmodel *model;
    ERROR error;
    int N, varCount, grbError, optimstatus;
    int * ind;
    double * val, *lb, *ub, *obj, *sol;
    char *vtype;
    VAR* varArr;
    env = NULL;
    model = NULL;
    ind = NULL;
    sol = val = lb = ub = obj = NULL;
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
    error = createVarArr(varArr, varCount, N, pBoard);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return error;
    }
    /* Create an empty model named "sudoku" */
    if (ilp) {
        setVtype(vtype, varCount, GRB_BINARY);
        createRandomObjFunction(obj, varCount); /*TODO: implement. Perhaps not random, but garbage obj?*/
        grbError = GRBnewmodel(env, &model, "sudoku", varCount, obj, NULL, NULL, vtype, NULL);
    }
    else{
        setVtype(vtype, varCount, GRB_CONTINUOUS);
        fillArrWithDouble(lb, varCount, 0.0);
        fillArrWithDouble(ub, varCount, 1.0);
        createRandomObjFunction(obj, 0);
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
    error = addCellConstraints(varArr, model, ind, val, varCount);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    error = addRowConstraints(model, varArr, ind, val, varCount, pBoard);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    error = addColConstraints(model, varArr, ind, val, varCount, pBoard);
    if (error != NO_ERROR){
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    error = addBlockConstraints(model, varArr, ind, val, varCount, pBoard);
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
    grbError = GRBoptimize(model);
    if (grbError){
        printf("ERROR %d GRBOptimize(): %s\n", grbError, GRBgeterrormsg(env));
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    grbError = GRBwrite(model, "sudoku.lp");
    if (grbError){
        printf("ERROR %d GRBWrite(): %s\n", grbError, GRBgeterrormsg(env));
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    grbError = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (optimstatus == GRB_OPTIMAL)
        printf("Optimal objective: found\n");
    /* no solution found */
    else if (optimstatus == GRB_INF_OR_UNBD)
        printf("Model is infeasible or unbounded\n");
        /* error or calculation stopped */
    else
        printf("Optimization was stopped early\n");
    /*TODO (omer): correct sol with malloc and everything */
    sol = val;
    grbError = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, varCount, sol);
    if (ilp){
        error = fillBoardFromSol(pBoard, varArr, varCount, sol);
    }
    else
        error = TMP_ERROR;/*TODO add options for LP not just ILP*/
    if (grbError){
        printf("ERROR %d GRBgetdblattrarray(): %s\n", grbError, GRBgeterrormsg(env));
        cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
        return GUROBI_ERROR;
    }
    cleanUp(ind, val, lb, ub, obj, vtype, varArr, env, model);
    return error;

}

ERROR fillBoardFromSol(board *pBoard, VAR *varArr, int varCount, double *sol) {
    int i;
    for (i = 0; i < varCount; i++) {
        if (sol[i] == 1) {
            setCell(pBoard, (varArr+i)->row, (varArr+i)->col, (varArr+i)->val);
        }
    }
    return NO_ERROR;
}

void createRandomObjFunction(double *obj, int varCount) {
    int i; /*TODO: changeme*/
    for (i = 0; i < varCount; i++) {
        obj[i] = (double) rand() / RAND_MAX;
    }

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
ERROR addBlockConstraints(GRBmodel *model, VAR *varArr, int *ind, double *val, int varCount, board *pBoard) {
    int rowLen, colLen, blockRow, blockCol, index, i,v, N, numOfCols, numOfRows, grbError;
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
                    if (((varArr+i)->row / rowLen) == blockRow &&
                        ((varArr+i)->col / colLen) == blockCol &&
                         (varArr+i)->val == v){
                        ind[index] = i;
                        index++;
                    }
                }
                if (index > 0){
                    grbError = GRBaddconstr(model, index, ind, val, GRB_EQUAL, 1.0, NULL);
                    if (grbError) {
                        printf("ERROR in block constraint!\n");
                        return GUROBI_ERROR;
                    }
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
ERROR addRowConstraints(GRBmodel *model, VAR *varArr, int *ind, double *val, int varCount, board *pBoard) {
    int N, row, index, i,v, grbError;
    N = pBoard->squareSideSize;
    for (row = 0; row < N; row++) {
        for (v = 1; v < N + 1; v++) {
            index = 0;
            for (i = 0; i < varCount; i++) {
                if ((varArr+i)->row == row && (varArr+i)->val == v){
                    ind[index] = i;
                    index++;
                }
            }
            if (index > 0){
                grbError = GRBaddconstr(model, index, ind, val, GRB_EQUAL, 1.0, NULL);
                if (grbError) {
                    printf("ERROR in row constraint!\n");
                    return GUROBI_ERROR;
                }
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
ERROR addColConstraints(GRBmodel *model, VAR *varArr, int *ind, double *val, int varCount, board *pBoard){
    int N, col, index, i,v, grbError;
    N = pBoard->squareSideSize;
    for (col = 0; col < N; col++) {
        for (v = 1; v < N + 1; v++) {
            index = 0;
            for (i = 0; i < varCount; i++) {
                if ((varArr+i)->col == col && (varArr+i)->val == v){
                    ind[index] = i;
                    index++;
                }
            }
            if (index > 0){
                grbError = GRBaddconstr(model, index, ind, val, GRB_EQUAL, 1.0, NULL);
                if (grbError) {
                    printf("ERROR in col constraint!\n");
                    return GUROBI_ERROR;
                }
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
 * @param varArr
 * @param model
 * @param ind
 * @param val
 * @param N
 * @param varCount
 * @return
 */
ERROR addCellConstraints(VAR *varArr, GRBmodel *model, int *ind, double *val, int varCount) {
    /*TODO: double check this logic*/
    int i, index, row, col, numOfVarsInConstraint, grbError;
    for (i = 0; i < varCount;) {
        numOfVarsInConstraint = 0;
        index = 0;
        row = (varArr+i)->row;
        col = (varArr+i)->col;
        for (; (varArr+i)->row == row && (varArr+i)->col == col; i++) { /*Since variables are in order of row and col*/
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
 * @param varArr pre-allocated array
 * @param varCount size of this array
 */
ERROR createVarArr(VAR *varArr, int varCount, int N, board *pBoard) {
    int i, j, v, index;
    index = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (emptyCell(pBoard, i, j)) {
                for (v = 1; v < N + 1; v++) {/*Values are 1-indexed*/
                    if (validAsignment(pBoard, v, i, j)) {
                        if (index > varCount) {
                            if (DEBUG) printf("VarCount exceeded in creating varArr\n");
                            return GUROBI_ERROR; /*Shouldn't happen */
                        }
                        (varArr+index)->row = i;
                        (varArr+index)->col = j;
                        (varArr+index)->val = v;
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
