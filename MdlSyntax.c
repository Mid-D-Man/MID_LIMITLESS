//te
public void Text1:int val,byte val2:{

if:val1 >= 15:{
    return;
}else if: val1 <15:{
    debunk;
}

}

public void Text2:int val1,byte val2{
    if:val1 >= 15{
        return;
    }else if:val2 <5{
debunk;
    }else{
return;
    }
}

public void Text3(int val1,byte val2){
    if(val1 >15){
        return;
    }else{
        debunk;
    }
}

public void Text4:int val1,byte val2:{
  if(val1 >15){
        return;
    }else{
        debunk;
    }
}

public class mine : system{
    systemCall Text1:15,4:
    systemCall Text2:1,18:
    systemCall Text3(12,2);
    systemCall Text4:9,8:
}