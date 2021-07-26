package com.example.phaml.smart_home_app;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class RegisterActivity extends AppCompatActivity {

    private EditText reguser,repass;
    private Button register;

    public static Details details;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        reguser = findViewById(R.id.reguser);
        repass = findViewById(R.id.regpass);
        register = findViewById(R.id.register);

        register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String regUsername = reguser.getText().toString();
                String regUsetpass = repass.getText().toString();

                if(validate(regUsername,regUsetpass)){
                    details = new Details(regUsername,regUsetpass);
                    startActivity(new Intent(RegisterActivity.this,LoginActivity.class));
                    Toast.makeText(RegisterActivity.this,"Đăng ký thành công",Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private boolean validate(String userName, String password){
        if(userName.isEmpty() || password.length() < 8){
            Toast.makeText(RegisterActivity.this,"Vui lòng nhập tất cả thông tin, mật khẩu có ít nhất 8 kí tự",Toast.LENGTH_SHORT).show();
            return false;
        }
        else{
            return true;
        }
    }
}
