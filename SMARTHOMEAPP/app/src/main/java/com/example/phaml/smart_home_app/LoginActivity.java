package com.example.phaml.smart_home_app;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class LoginActivity extends AppCompatActivity {

    private EditText Name, Password;
    private TextView epass;
    private Button Login;
    private int counter = 5;
    boolean isValid;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        Name = findViewById(R.id.username);
        Password = findViewById(R.id.password);
        Login = findViewById(R.id.btnlogin);
        epass = findViewById(R.id.epass);

        //so sánh điều kiện login
        Login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String inputUsername = Name.getText().toString();
                String inputPassword = Password.getText().toString();

                if (inputUsername.isEmpty() || inputPassword.isEmpty()){
                    Toast.makeText(LoginActivity.this, "Xin vui lòng nhập Tên đăng nhập và Mật khẩu", Toast.LENGTH_SHORT).show();
                }
                else{
                    isValid = validate(inputUsername,inputPassword);

                    if(!isValid){
                        counter--;

                        Toast.makeText(LoginActivity.this, "Sai tên đăng nhập hoặc mật khẩu. Bạn còn " + String.valueOf(counter) + " lần nhập",Toast.LENGTH_SHORT).show();

                        if(counter == 0){
                            Login.setEnabled(false);
                        }
                    }
                    else {
                        Toast.makeText(LoginActivity.this,"Đăng nhập thành công",Toast.LENGTH_SHORT).show();

                        // nếu đúng thì into next activity
                        startActivity(new Intent(LoginActivity.this, MainActivity.class));//bắt đầu activity 2
                    }
                }
            }
        });
    }

    private boolean validate(String userName, String userPassword){
        if (RegisterActivity.details != null){
            if(userName.equals(RegisterActivity.details.getUsername()) && userPassword.equals(RegisterActivity.details.getPassword())){
                return true;
            }
        }
            return false;
    }
}


